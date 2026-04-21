package com.margelo.nitro.rngine

import android.os.Handler
import android.os.HandlerThread
import android.view.Choreographer
import com.facebook.react.uimanager.ThemedReactContext
import java.nio.ByteBuffer

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  private external fun initializeLoop(buffer: ByteBuffer, count: Int)
  private external fun pauseLoop()
  private external fun resumeLoop()

  private var renderThread: HandlerThread? = null
  private var renderHandler: Handler? = null

  private val frameCallback = object : Choreographer.FrameCallback {
    override fun doFrame(frameTimeNanos: Long) {
      if (!isPaused) {
        renderHandler?.post { view.drawFrame() }
        Choreographer.getInstance().postFrameCallback(this)
      }
    }
  }

  override var isPaused: Boolean = true
    set(value) {
      field = value
      if (value) {
        Choreographer.getInstance().removeFrameCallback(frameCallback)
        pauseLoop()
      } else {
        Choreographer.getInstance().postFrameCallback(frameCallback)
        resumeLoop()
      }
    }

  override var initialEntities: Array<Entity> = emptyArray()

  override val view: GameView = GameView(
    context
  ).apply {
    onAttached = {
      val initialEntitiesBuffer = EntitySerializer.encode(initialEntities)

      initializeLoop(initialEntitiesBuffer, initialEntities.size)

      renderThread = HandlerThread("RenderThread").also { it.start() }
      renderHandler = Handler(renderThread!!.looper)

      if (!isPaused) {
        Choreographer.getInstance().postFrameCallback(frameCallback)
        resumeLoop()
      }
    }
    onDetached = {
      Choreographer.getInstance().removeFrameCallback(frameCallback)
      renderThread?.quitSafely()
      renderThread = null
      renderHandler = null
    }
  }
}
