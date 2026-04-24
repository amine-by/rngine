package com.margelo.nitro.rngine

import android.os.Handler
import android.os.HandlerThread
import android.view.Choreographer
import com.facebook.react.uimanager.ThemedReactContext

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  private var renderThread: HandlerThread? = null
  private var renderHandler: Handler? = null

  private val frameCallback = object : Choreographer.FrameCallback {
    override fun doFrame(frameTimeNanos: Long) {
      renderHandler?.post { view.drawFrame() }
      Choreographer.getInstance().postFrameCallback(this)
    }
  }

  override val view: GameView = GameView(
    context
  ).apply {
    onAttached = {
      renderThread = HandlerThread("RenderThread").also { it.start() }
      renderHandler = Handler(renderThread!!.looper)
      Choreographer.getInstance().postFrameCallback(frameCallback)
    }
    onDetached = {
      Choreographer.getInstance().removeFrameCallback(frameCallback)
      renderThread?.quitSafely()
      renderThread = null
      renderHandler = null
    }
  }
}
