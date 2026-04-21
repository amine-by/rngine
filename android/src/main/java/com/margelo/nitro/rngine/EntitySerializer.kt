package com.margelo.nitro.rngine

import java.nio.ByteBuffer
import java.nio.ByteOrder

object EntitySerializer {
  private const val ENTITY_ID_SIZE = 40
  private const val ENTITY_BUFFER_SIZE = ENTITY_ID_SIZE + 8 * 6

  fun encode(entities: Array<Entity>): ByteBuffer {
    val buffer = ByteBuffer
      .allocateDirect(entities.size * ENTITY_BUFFER_SIZE)
      .order(ByteOrder.nativeOrder())

    entities.forEach { entity ->
      val idBytes = entity.id.toByteArray(Charsets.UTF_8).copyOf(ENTITY_ID_SIZE)
      buffer.put(idBytes)
      buffer.putDouble(entity.px)
      buffer.putDouble(entity.py)
      buffer.putDouble(entity.width)
      buffer.putDouble(entity.height)
      buffer.putDouble(entity.vx)
      buffer.putDouble(entity.vy)
    }

    buffer.rewind()

    return buffer
  }

  fun decode(buffer: ByteBuffer): ArrayList<Entity> {
    buffer.order(ByteOrder.nativeOrder())
    val entities = ArrayList<Entity>()

    while (buffer.remaining() >= ENTITY_BUFFER_SIZE) {
      val idBytes = ByteArray(ENTITY_ID_SIZE)
      buffer.get(idBytes)

      val id = idBytes.toString(Charsets.UTF_8).trimEnd('\u0000')

      val px = buffer.double
      val py = buffer.double
      val width = buffer.double
      val height = buffer.double
      val vx = buffer.double
      val vy = buffer.double

      entities.add(Entity(id, px, py, width, height, vx, vy))
    }
    return entities
  }
}
