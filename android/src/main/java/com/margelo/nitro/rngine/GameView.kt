package com.margelo.nitro.rngine

import android.content.Context
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.SurfaceView
import android.view.View
import java.nio.ByteBuffer

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
    val scaleX = canvas.width / worldRect.right.toFloat()
    val scaleY = canvas.height / worldRect.bottom.toFloat()
    val scale = minOf(scaleX, scaleY)

    val offsetX = (canvas.width - worldRect.right.toFloat() * scale) / 2f
    val offsetY = (canvas.height - worldRect.bottom.toFloat() * scale) / 2f

    rects.forEach { rect ->
      paint.color = rect.color
      canvas.drawRect(
        rect.left.toFloat() * scale + offsetX,
        rect.top.toFloat() * scale + offsetY,
        rect.right.toFloat() * scale + offsetX,
        rect.bottom.toFloat() * scale + offsetY,
        paint
      )
    }
    holder.unlockCanvasAndPost(canvas)
  }
}
