package com.margelo.nitro.rngine

import java.nio.ByteBuffer
import java.nio.ByteOrder

object SnapshotSerializer {
  fun decode(bytes: ByteArray): Snapshot {
    val buffer = ByteBuffer.wrap(bytes).order(ByteOrder.nativeOrder())

    val width = buffer.float
    val height = buffer.float
    val color = buffer.int
    val asset = buffer.int
    val progress = if (asset < 0) buffer.float else null

    val screen = Screen(width, height, color, asset, progress)
    val rects = ArrayList<Rect>()

    while (buffer.remaining() >= 24) {
      val left = buffer.float
      val right = buffer.float
      val top = buffer.float
      val bottom = buffer.float
      val color = buffer.int
      val asset = buffer.int
      val progress = if (asset < 0) buffer.float else null

      rects.add(Rect(left, right, top, bottom, color, asset, progress))
    }

    return Snapshot(screen, rects)
  }
}
