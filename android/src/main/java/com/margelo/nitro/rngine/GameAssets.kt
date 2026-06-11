package com.margelo.nitro.rngine

import android.util.Log
import com.airbnb.lottie.LottieCompositionFactory
import com.airbnb.lottie.LottieDrawable
import com.caverock.androidsvg.SVG
import java.net.URL

class GameAssets : HybridGameAssetsSpec() {
  private external fun registerLottieDuration(id: Double, duration: Double)

  companion object {
    private val assetCache = HashMap<Int, Asset>()
    fun getAsset(id: Int): Asset? = assetCache[id]
  }

  override fun isAssetRegistered(id: Double): Boolean {
    return id.toInt() in assetCache
  }

  override fun registerSvg(id: Double, uri: String) {
    try {
      val stream = URL(uri).openStream()
      assetCache[id.toInt()] = Asset.Svg(SVG.getFromInputStream(stream).renderToPicture())
      Log.d("GameAssets", "assetCache: $assetCache")
    } catch (e: Exception) {
      Log.e("GameAssets", "Failed to parse SVG: $uri", e)
    }
  }

  override fun registerLottie(id: Double, json: String) {
    try {
      val composition = LottieCompositionFactory.fromJsonStringSync(json, null).value
      if (composition == null) {
        Log.e("GameAssets", "Failed to parse Lottie JSON for id: $id")
        return
      }
      val drawable = LottieDrawable()
      drawable.composition = composition
      assetCache[id.toInt()] = Asset.Lottie(drawable)
      registerLottieDuration(id, composition.duration.toDouble() / 1000)
      Log.d("GameAssets", "assetCache: $assetCache")
    } catch (e: Exception) {
      Log.e("GameAssets", "Failed to register Lottie: $id", e)
    }
  }
}
