package com.margelo.nitro.rngine

import android.content.Context
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class GameView(
  context: Context,
) : SurfaceView(
  context,
),
  SurfaceHolder.Callback {
  private external fun nativeOnSurfaceCreated(surface: Surface)

  private external fun nativeOnSurfaceChanged(
    width: Int,
    height: Int,
  )

  private external fun nativeOnSurfaceDestroyed()

  init {
    holder.addCallback(this)
  }

  override fun surfaceCreated(holder: SurfaceHolder) {
    nativeOnSurfaceCreated(holder.surface)
  }

  override fun surfaceChanged(
    holder: SurfaceHolder,
    format: Int,
    width: Int,
    height: Int,
  ) {
    nativeOnSurfaceChanged(width, height)
  }

  override fun surfaceDestroyed(holder: SurfaceHolder) {
    nativeOnSurfaceDestroyed()
  }
}
