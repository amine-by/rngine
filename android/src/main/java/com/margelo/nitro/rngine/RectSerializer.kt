package com.margelo.nitro.rngine

import java.nio.ByteBuffer
import java.nio.ByteOrder

object RectSerializer {
  private const val RECT_SIZE = 4 * 4 + 4 * 2
  fun decode(buffer: ByteBuffer): ArrayList<Rect> {
    buffer.order(ByteOrder.nativeOrder())
    val rects = ArrayList<Rect>()

    while (buffer.remaining() >= RECT_SIZE) {
      val left = buffer.float
      val right = buffer.float
      val top = buffer.float
      val bottom = buffer.float
      val color = buffer.int
      val asset = buffer.int

      rects.add(Rect(left, right, top, bottom, color, asset))
    }
    return rects
  }
}
