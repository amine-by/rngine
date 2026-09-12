package com.margelo.nitro.rngine

import android.util.Log
import androidx.annotation.Keep
import com.margelo.nitro.NitroModules
import java.io.IOException
import java.net.URL

@Keep
object ImageLoaderHelper {
  @JvmStatic
  fun loadFromUrl(uri: String): ByteArray {
    Log.d("ImageLoaderHelper", "loadFromUrl: fetching $uri")
    return try {
      val bytes = URL(uri).openStream().use { it.readBytes() }
      Log.d("ImageLoaderHelper", "loadFromUrl: success, ${bytes.size} bytes for $uri")
      bytes
    } catch (e: IOException) {
      Log.e("ImageLoaderHelper", "loadFromUrl: network/IO failure for $uri", e)
      throw e
    } catch (e: Exception) {
      Log.e("ImageLoaderHelper", "loadFromUrl: unexpected failure for $uri", e)
      throw e
    }
  }

  @JvmStatic
  fun loadFromResource(uri: String): ByteArray {
    Log.d("ImageLoaderHelper", "loadFromResource: resolving resource for '$uri'")
    val context = NitroModules.applicationContext
    if (context == null) {
      Log.e(
        "ImageLoaderHelper",
        "loadFromResource: applicationContext is null, NitroModules not initialized?"
      )
      throw IllegalStateException("No Context available!")
    }
    var resId = context.resources.getIdentifier(uri, "raw", context.packageName)
    if (resId == 0) {
      resId = context.resources.getIdentifier(uri, "drawable", context.packageName)
    }
    if (resId == 0) {
      Log.e("ImageLoaderHelper", "loadFromResource: no resource found for '$uri'")
      throw IllegalStateException("Could not resolve raw resource for asset: $uri")
    }
    Log.d("ImageLoaderHelper", "loadFromResource: resolved '$uri' -> resId=$resId")
    return try {
      val bytes = context.resources.openRawResource(resId).use { it.readBytes() }
      Log.d("ImageLoaderHelper", "loadFromResource: success, ${bytes.size} bytes for '$uri'")
      bytes
    } catch (e: Exception) {
      Log.e("ImageLoaderHelper", "loadFromResource: failed reading resId=$resId for '$uri'", e)
      throw e
    }
  }
}
