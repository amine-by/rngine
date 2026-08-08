package com.margelo.nitro.rngine

import android.content.Context
import android.graphics.Color
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
    color = Color.TRANSPARENT
    style = Paint.Style.FILL
  }

  private fun drawRenderable(
    canvas: android.graphics.Canvas,
    left: Float,
    top: Float,
    right: Float,
    bottom: Float,
    color: Int,
    asset: Int,
    progress: Float?,
    clampedLeft: Float = left,
    clampedTop: Float = top,
    clampedRight: Float = right,
    clampedBottom: Float = bottom,
  ) {
    paint.color = color
    canvas.drawRect(clampedLeft, clampedTop, clampedRight, clampedBottom, paint)

    canvas.withClip(clampedLeft, clampedTop, clampedRight, clampedBottom) {
      withTranslation(left, top) {
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

    drawRenderable(
      canvas,
      screenLeft,
      screenTop,
      screenRight,
      screenBottom,
      snapshot.screen.color,
      snapshot.screen.asset,
      snapshot.screen.progress,
    )

    snapshot.rects.forEach { rect ->
      val left = rect.left * scale + screenLeft
      val top = rect.top * scale + screenTop
      val right = rect.right * scale + screenLeft
      val bottom = rect.bottom * scale + screenTop

      val clampedLeft = left.coerceAtLeast(screenLeft)
      val clampedTop = top.coerceAtLeast(screenTop)
      val clampedRight = right.coerceAtMost(screenLeft + snapshot.screen.width * scale)
      val clampedBottom = bottom.coerceAtMost(screenTop + snapshot.screen.height * scale)

      drawRenderable(
        canvas,
        left,
        top,
        right,
        bottom,
        rect.color,
        rect.asset,
        rect.progress,
        clampedLeft,
        clampedTop,
        clampedRight,
        clampedBottom
      )
    }
    holder.unlockCanvasAndPost(canvas)
  }
}
