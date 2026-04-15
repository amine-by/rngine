package com.margelo.nitro.rngine

import java.nio.ByteBuffer
import java.nio.ByteOrder

object EntitySerializer {
  private const val ENTITY_ID_SIZE = 37
  private const val ENTITY_BUFFER_SIZE = 69

  fun encode(entities: Array<Entity>): ByteBuffer {
    val buffer = ByteBuffer
      .allocateDirect(entities.size * ENTITY_BUFFER_SIZE)
      .order(ByteOrder.nativeOrder())

    entities.forEach { entity ->
      val idBytes = entity.id.toByteArray(Charsets.UTF_8).copyOf(ENTITY_ID_SIZE)
      buffer.put(idBytes)
      buffer.putDouble(entity.x)
      buffer.putDouble(entity.y)
      buffer.putDouble(entity.width)
      buffer.putDouble(entity.height)
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

      val x = buffer.double
      val y = buffer.double
      val width = buffer.double
      val height = buffer.double

      entities.add(Entity(id, x, y, width, height))
    }
    return entities
  }
}
