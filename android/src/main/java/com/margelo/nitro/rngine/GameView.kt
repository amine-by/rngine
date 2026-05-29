package com.margelo.nitro.rngine

import android.content.Context
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.SurfaceView
import android.view.View
import java.nio.ByteBuffer
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
  private external fun getRectsSnapshot(): ByteBuffer
  var onAttached: () -> Unit = {}
  var onDetached: () -> Unit = {}

  private val paint = Paint().apply {
    color = Color.TRANSPARENT
    style = Paint.Style.FILL
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

    val rects = RectSerializer.decode(getRectsSnapshot())

    val worldRect = rects.first()
    val scaleX = canvas.width / worldRect.right
    val scaleY = canvas.height / worldRect.bottom
    val scale = minOf(scaleX, scaleY)

    val offsetX = (canvas.width - worldRect.right * scale) / 2f
    val offsetY = (canvas.height - worldRect.bottom * scale) / 2f

    rects.forEach { rect ->
      val left = rect.left * scale + offsetX
      val top = rect.top * scale + offsetY
      val right = rect.right * scale + offsetX
      val bottom = rect.bottom * scale + offsetY

      val clampedLeft = left.coerceAtLeast(offsetX)
      val clampedTop = top.coerceAtLeast(offsetY)
      val clampedRight = right.coerceAtMost(offsetX + worldRect.right * scale)
      val clampedBottom = bottom.coerceAtMost(offsetY + worldRect.bottom * scale)

      paint.color = rect.color
      canvas.drawRect(
        clampedLeft,
        clampedTop,
        clampedRight,
        clampedBottom,
        paint
      )

      val picture = GameAssets.getPicture(rect.asset)
      if (picture != null) {
        canvas.withClip(clampedLeft, clampedTop, clampedRight, clampedBottom) {
          withTranslation(left, top) {
            scale(
              (right - left) / picture.width,
              (bottom - top) / picture.height
            )
            drawPicture(picture)
          }
        }
      }
    }
    holder.unlockCanvasAndPost(canvas)
  }
}
