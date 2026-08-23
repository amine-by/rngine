package com.margelo.nitro.rngine

sealed class Shape(
  val color: Int,
  val asset: Int,
  val progress: Float?,
) {
  class Rect(
    val left: Float,
    val right: Float,
    val top: Float,
    val bottom: Float,
    color: Int,
    asset: Int,
    progress: Float?,
  ) : Shape(color, asset, progress)

  class Circle(
    val px: Float,
    val py: Float,
    val radius: Float,
    color: Int,
    asset: Int,
    progress: Float?,
  ) : Shape(color, asset, progress)
}
