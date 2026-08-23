package com.margelo.nitro.rngine

import android.util.Log
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
    val shapes = ArrayList<Shape>()

    while (buffer.hasRemaining()) {
      when (val shapeType = buffer.get().toInt()) {
        0 -> {
          val left = buffer.float
          val right = buffer.float
          val top = buffer.float
          val bottom = buffer.float

          val color = buffer.int
          val asset = buffer.int
          val progress = if (asset < 0) buffer.float else null

          shapes.add(Shape.Rect(left, right, top, bottom, color, asset, progress))
        }

        1 -> {
          val px = buffer.float
          val py = buffer.float
          val radius = buffer.float

          val color = buffer.int
          val asset = buffer.int
          val progress = if (asset < 0) buffer.float else null

          shapes.add(Shape.Circle(px, py, radius, color, asset, progress))
        }

        else -> Log.e("SnapshotSerializer", "Unknown shapeType: $shapeType")
      }
    }

    return Snapshot(screen, shapes)
  }
}
