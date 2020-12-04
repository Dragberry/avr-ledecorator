package org.dragberry.ledecorator.apps.sandbox

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Matrix
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.widget.ImageView
import org.dragberry.ledecorator.utils.Color
import org.dragberry.ledecorator.utils.Colors
import kotlin.math.roundToInt

const val FIELD_WIDTH = 32
const val FIELD_HEIGHT = 16

private const val TAG = "FieldView"


class FieldView(context: Context, attributesSet: AttributeSet) :
    ImageView(context, attributesSet) {

    private var onDrawPoint: ((x: Int, y: Int, color: Color) -> Unit)? = null

    var currentColor: Color = Colors.WHITE

    private val bitmap: Bitmap = Bitmap.createBitmap(FIELD_HEIGHT, FIELD_WIDTH, Bitmap.Config.ARGB_8888).apply {
        setImageBitmap(this)
        eraseColor(Colors.BLACK.real)
    }

    private val inverseMatrix: Matrix = Matrix()
    private val points = FloatArray(2)

    fun clearScreen(color: Color) {
        bitmap.eraseColor(color.real)
    }

    fun setPixel(x: Int, y: Int, color: Int) {
        Log.i(TAG, "X=${FIELD_HEIGHT - y - 1}   Y=$x Color=$color")
        bitmap.setPixel(FIELD_HEIGHT- y - 1, x, color)
    }

    fun getPixel(x: Int, y: Int): Int = bitmap.getPixel(y, FIELD_HEIGHT - x - 1)

    fun onDrawPoint(onDrawPoint: (x: Int, y: Int, color: Color) -> Unit) {
        this.onDrawPoint = onDrawPoint
    }

    override fun performClick(): Boolean {
        super.performClick()
        return true
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        if (event == null) return true

        return when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                true
            }
            MotionEvent.ACTION_UP -> {
                performAction(event)
                performClick()
                true
            }
            MotionEvent.ACTION_MOVE -> {
                performAction(event)
                true
            }
            else -> true
        }
    }

    private fun performAction(event: MotionEvent) {
        imageMatrix.invert(inverseMatrix)
        points[0] = event.x
        points[1] = event.y
        inverseMatrix.mapPoints(points)
        val x = points[0].roundToInt()
        val y = points[1].roundToInt()
        Log.i(TAG, "x=$x, Y=$y")

        if (y in 0 until FIELD_WIDTH && x in 0 until FIELD_HEIGHT) {
            val previousColor: Int = bitmap.getPixel(x, y)
            if (previousColor != currentColor.real) {
                onDrawPoint?.let { it(y, FIELD_HEIGHT - x - 1, currentColor) }
                bitmap.setPixel(x, y, currentColor.real)
                postInvalidate()
            }

        }
    }
}