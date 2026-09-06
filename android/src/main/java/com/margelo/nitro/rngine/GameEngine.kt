package com.margelo.nitro.rngine

import com.facebook.react.uimanager.ThemedReactContext

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  override val view: GameView = GameView(context)
}
