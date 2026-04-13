package com.margelo.nitro.rngine

import android.content.Context
import android.util.AttributeSet
import android.view.View
class GameView(
  context: Context,
  attrs: AttributeSet? = null,
  defStyleAttr: Int = 0,
) : View(
  context,
  attrs,
  defStyleAttr,
) {
  var onAttached: () -> Unit = {}
  var onDetached: () -> Unit = {}

  init {
    addOnAttachStateChangeListener(
      object : OnAttachStateChangeListener {
        override fun onViewAttachedToWindow(view: View) = onAttached()
        override fun onViewDetachedFromWindow(view: View) = onDetached()
      })
  }
}
