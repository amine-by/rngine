package com.margelo.nitro.rngine

import android.graphics.Picture
import android.util.Log
import com.caverock.androidsvg.SVG
import java.net.URL

class GameAssets : HybridGameAssetsSpec() {
  companion object {
    private val pictureCache = HashMap<Double, Picture>()
    fun getPicture(id: Double): Picture? = pictureCache[id]
  }
  override fun isAssetRegistered(id: Double): Boolean {
    return id in pictureCache
  }

  override fun registerAsset(id: Double, uri: String) {
    try {
      val stream = URL(uri).openStream()
      pictureCache[id] = SVG.getFromInputStream(stream).renderToPicture()
      Log.d("GameAssets", "pictureCache: $pictureCache")
    } catch (e: Exception) {
      Log.e("GameAssets", "Failed to parse SVG: $uri", e)
    }
  }

}
