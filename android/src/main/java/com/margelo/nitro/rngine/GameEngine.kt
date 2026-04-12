package com.margelo.nitro.rngine

import android.graphics.Color
import android.view.View
import com.facebook.react.uimanager.ThemedReactContext

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  private external fun createLoop()
  private external fun destroyLoop()
  private external fun pauseLoop()
  private external fun resumeLoop()

  override var isPaused: Boolean = false
    set(value) {
      if (field == value) return
      field = value
      if (value) {
        pauseLoop()
        view.setBackgroundColor(Color.RED)
      } else {
        resumeLoop()
        view.setBackgroundColor(Color.GREEN)
      }
    }

  override val view: View = View(context).apply {
    addOnAttachStateChangeListener(object : View.OnAttachStateChangeListener {
      override fun onViewAttachedToWindow(v: View) {
        createLoop()
      }

      override fun onViewDetachedFromWindow(v: View) {
        destroyLoop()
      }
    })
  }
}
