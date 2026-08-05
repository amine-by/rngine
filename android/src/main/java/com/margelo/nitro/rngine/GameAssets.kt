package com.margelo.nitro.rngine

import android.util.Log
import com.airbnb.lottie.LottieCompositionFactory
import com.airbnb.lottie.LottieDrawable
import com.caverock.androidsvg.SVG
import com.margelo.nitro.core.Promise
import java.net.URL
import com.margelo.nitro.NitroModules

class GameAssets : HybridGameAssetsSpec() {
  private external fun registerLottieDuration(id: Double, duration: Double)

  companion object {
    private val assetCache = HashMap<Int, Asset>()
    fun getAsset(id: Int): Asset? = assetCache[id]
  }

  override fun isAssetRegistered(id: Double): Boolean {
    return id.toInt() in assetCache
  }

  override fun registerSvg(id: Double, uri: String): Promise<Unit> {
    return Promise.async {
      try {
        val stream = if (uri.startsWith("http://") || uri.startsWith("https://")) {
          URL(uri).openStream()
        } else {
          val context = NitroModules.applicationContext ?: throw Error("No Context available!")
          val resId = context.resources.getIdentifier(uri, "raw", context.packageName)
          if (resId == 0) {
            throw IllegalStateException("Could not resolve drawable resource for asset: $uri")
          }
          context.resources.openRawResource(resId)
        }
        assetCache[id.toInt()] = Asset.Svg(SVG.getFromInputStream(stream).renderToPicture())
        Log.d("GameAssets", "assetCache: $assetCache")
      } catch (e: Exception) {
        Log.e("GameAssets", "Failed to parse SVG: $uri", e)
        throw e
      }
    }
  }

  override fun registerLottie(id: Double, json: String): Promise<Unit> {
    return Promise.async {
      try {
        val composition = LottieCompositionFactory.fromJsonStringSync(json, null).value
          ?: throw IllegalStateException("Failed to parse Lottie JSON for id: $id")
        val drawable = LottieDrawable()
        drawable.composition = composition
        assetCache[id.toInt()] = Asset.Lottie(drawable)
        registerLottieDuration(id, composition.duration.toDouble() / 1000)
        Log.d("GameAssets", "assetCache: $assetCache")
      } catch (e: Exception) {
        Log.e("GameAssets", "Failed to register Lottie: $id", e)
        throw e
      }
    }
  }
}
