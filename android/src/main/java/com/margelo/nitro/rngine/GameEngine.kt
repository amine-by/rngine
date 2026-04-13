package com.margelo.nitro.rngine

import com.facebook.react.uimanager.ThemedReactContext
import java.nio.ByteBuffer
import java.nio.ByteOrder

class GameEngine(val context: ThemedReactContext) : HybridGameEngineSpec() {
  private external fun createLoop(buffer: ByteBuffer, count: Int)
  private external fun destroyLoop()
  private external fun pauseLoop()
  private external fun resumeLoop()
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
      val buffer = ByteBuffer
        .allocateDirect(initialEntities.size * 69)
        .order(ByteOrder.nativeOrder())
      initialEntities.forEach { entity ->
        val idBytes = entity.id.toByteArray(Charsets.UTF_8).copyOf(37)
        buffer.put(idBytes)
        buffer.putDouble(entity.x)
        buffer.putDouble(entity.y)
        buffer.putDouble(entity.width)
        buffer.putDouble(entity.height)
      }
      buffer.rewind()

      createLoop(buffer, initialEntities.size)
      if (!isPaused) resumeLoop()
    }
    onDetached = {
      destroyLoop()
    }
  }
}
