package com.margelo.nitro.rngine

import android.content.Context
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.util.Log
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

  private external fun getEntitiesSnapshot(): ByteBuffer

  var onAttached: () -> Unit = {}
  var onDetached: () -> Unit = {}

  private val paint = Paint().apply {
    color = Color.RED
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
    canvas.drawColor(Color.WHITE)
    val entities = EntitySerializer.decode(getEntitiesSnapshot())

    entities.forEach { entity ->
      val x = entity.x.toFloat()
      val y = entity.y.toFloat()
      val width = entity.width.toFloat()
      val height = entity.height.toFloat()

      canvas.drawRect(
        x,
        y,
        x + width,
        y + height,
        paint
      )
    }
    holder.unlockCanvasAndPost(canvas)
  }
}
