package com.margelo.nitro.rngine

import android.util.Log
import com.facebook.react.uimanager.ThemedReactContext
import java.nio.ByteBuffer

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  private external fun createLoop(buffer: ByteBuffer, count: Int)
  private external fun destroyLoop()
  private external fun pauseLoop()
  private external fun resumeLoop()

  private var renderThread: Thread? = null

  override var isPaused: Boolean = true
    set(value) {
      field = value
      if (value)
        pauseLoop()
      else
        resumeLoop()
    }

  override var initialEntities: Array<Entity> = emptyArray()

  override val view: GameView = GameView(
    context
  ).apply {
    onAttached = {
      val initialEntitiesBuffer = EntitySerializer.encode(initialEntities)

      createLoop(initialEntitiesBuffer, initialEntities.size)

      renderThread = Thread {
        while (!Thread.currentThread().isInterrupted) {
          drawFrame()
          try {
            Thread.sleep(16)
          } catch (e: InterruptedException) {
            Log.e("GameEngine", "Render thread interrupted", e)
            break
          }
        }
      }
      renderThread?.start()

      if (!isPaused) resumeLoop()
    }
    onDetached = {
      destroyLoop()
      renderThread?.interrupt()
      renderThread = null
    }
  }
}
