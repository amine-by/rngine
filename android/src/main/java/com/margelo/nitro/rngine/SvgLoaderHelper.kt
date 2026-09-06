package com.margelo.nitro.rngine

import androidx.annotation.Keep
import com.margelo.nitro.NitroModules
import java.net.URL

@Keep
object SvgLoaderHelper {
  @JvmStatic
  fun loadFromUrl(uri: String): ByteArray {
    return URL(uri).openStream().use { it.readBytes() }
  }

  @JvmStatic
  fun loadFromResource(uri: String): ByteArray {
    val context = NitroModules.applicationContext
      ?: throw IllegalStateException("No Context available!")
    val resId = context.resources.getIdentifier(uri, "raw", context.packageName)
    if (resId == 0) {
      throw IllegalStateException("Could not resolve raw resource for asset: $uri")
    }
    return context.resources.openRawResource(resId).use { it.readBytes() }
  }
}
