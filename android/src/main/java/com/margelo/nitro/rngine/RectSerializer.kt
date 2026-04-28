package com.margelo.nitro.rngine

import java.nio.ByteBuffer
import java.nio.ByteOrder

object RectSerializer {
  private const val RECT_SIZE = 4 * 8 + 4
  fun decode(buffer: ByteBuffer): ArrayList<Rect> {
    buffer.order(ByteOrder.nativeOrder())
    val rects = ArrayList<Rect>()

    while (buffer.remaining() >= RECT_SIZE) {
      val left = buffer.double
      val right = buffer.double
      val top = buffer.double
      val bottom = buffer.double
      val color = buffer.int

      rects.add(Rect(left, right, top, bottom, color))
    }
    return rects
  }
}
