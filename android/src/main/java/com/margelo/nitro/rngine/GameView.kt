package com.margelo.nitro.rngine

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.util.AttributeSet
import android.view.SurfaceView
import android.view.View
import androidx.core.graphics.withTranslation
import androidx.core.graphics.withClip

class GameView(
  context: Context,
  attrs: AttributeSet? = null,
  defStyleAttr: Int = 0,
) : SurfaceView(
  context,
  attrs,
  defStyleAttr,
) {
  private external fun getSnapshot(): ByteArray
  var onAttached: () -> Unit = {}
  var onDetached: () -> Unit = {}

  private val paint = Paint().apply {
    style = Paint.Style.FILL
  }

  private fun drawAsset(
    canvas: Canvas,
    left: Float,
    top: Float,
    right: Float,
    bottom: Float,
    asset: Int,
    progress: Float?,
  ) {
    canvas.withTranslation(left, top) {
      when (val resolvedAsset = GameAssets.getAsset(asset)) {
        is Asset.Svg -> {
          scale(
            (right - left) / resolvedAsset.picture.width,
            (bottom - top) / resolvedAsset.picture.height
          )
          drawPicture(resolvedAsset.picture)
        }

        is Asset.Lottie -> {
          progress?.let {
            resolvedAsset.drawable.progress = it
          }
          resolvedAsset.drawable.setBounds(0, 0, (right - left).toInt(), (bottom - top).toInt())
          resolvedAsset.drawable.draw(canvas)
        }

        null -> {}
      }
    }
  }

  private fun drawRect(
    canvas: Canvas,
    left: Float,
    top: Float,
    right: Float,
    bottom: Float,
    color: Int,
    asset: Int,
    progress: Float?,
  ) {
    paint.color = color
    canvas.drawRect(left, top, right, bottom, paint)

    drawAsset(
      canvas,
      left,
      top,
      right,
      bottom,
      asset,
      progress
    )
  }

  private fun drawCircle(
    canvas: Canvas,
    cx: Float,
    cy: Float,
    radius: Float,
    color: Int,
    asset: Int,
    progress: Float?,
  ) {
    val left = cx - radius
    val top = cy - radius
    val right = cx + radius
    val bottom = cy + radius

    paint.color = color
    canvas.drawCircle(cx, cy, radius, paint)

    drawAsset(
      canvas,
      left,
      top,
      right,
      bottom,
      asset,
      progress,
    )
  }

  init {
    addOnAttachStateChangeListener(
      object : OnAttachStateChangeListener {
        override fun onViewAttachedToWindow(view: View) = onAttached()
        override fun onViewDetachedFromWindow(view: View) = onDetached()
      })
  }

  fun drawFrame() {
    if (!holder.surface.isValid) return
    val canvas = holder.lockCanvas() ?: return
    val snapshot = SnapshotSerializer.decode(getSnapshot())

    val scaleX = canvas.width / snapshot.screen.width
    val scaleY = canvas.height / snapshot.screen.height
    val scale = minOf(scaleX, scaleY)

    val screenLeft = (canvas.width - snapshot.screen.width * scale) / 2f
    val screenTop = (canvas.height - snapshot.screen.height * scale) / 2f
    val screenRight = screenLeft + snapshot.screen.width * scale
    val screenBottom = screenTop + snapshot.screen.height * scale

    drawRect(
      canvas,
      screenLeft,
      screenTop,
      screenRight,
      screenBottom,
      snapshot.screen.color,
      snapshot.screen.asset,
      snapshot.screen.progress,
    )

    canvas.withClip(screenLeft, screenTop, screenRight, screenBottom) {
      snapshot.shapes.forEach { shape ->
        when (shape) {
          is Shape.Rect -> {
            val left = shape.left * scale + screenLeft
            val top = shape.top * scale + screenTop
            val right = shape.right * scale + screenLeft
            val bottom = shape.bottom * scale + screenTop

            drawRect(
              canvas,
              left,
              top,
              right,
              bottom,
              shape.color,
              shape.asset,
              shape.progress,
            )
          }

          is Shape.Circle -> {
            val cx = shape.px * scale + screenLeft
            val cy = shape.py * scale + screenTop
            val radius = shape.radius * scale

            drawCircle(
              canvas,
              cx,
              cy,
              radius,
              shape.color,
              shape.asset,
              shape.progress,
            )
          }
        }
      }
    }
    holder.unlockCanvasAndPost(canvas)
  }
}
