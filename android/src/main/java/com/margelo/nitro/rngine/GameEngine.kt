package com.margelo.nitro.rngine

import android.graphics.Color
import android.view.View
import com.facebook.react.uimanager.ThemedReactContext

class GameEngine(val context: ThemedReactContext): HybridGameEngineSpec() {
  override val view: View = View(context).apply {
    setBackgroundColor(Color.BLUE)
  }
}
