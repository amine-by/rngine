package com.margelo.nitro.rngine

import android.graphics.Picture
import com.airbnb.lottie.LottieDrawable

sealed class Asset {
  class Svg(val picture: Picture) : Asset()
  class Lottie(val drawable: LottieDrawable) : Asset()
}
