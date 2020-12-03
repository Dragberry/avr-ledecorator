package org.dragberry.ledecorator.apps.sandbox


import android.content.Context
import android.graphics.Bitmap
import android.graphics.Color
import android.graphics.Matrix
import android.os.Bundle
import android.util.AttributeSet
import android.util.Log
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ImageView
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.utils.SelectColorDialogFragment
import java.util.*
import java.util.concurrent.ConcurrentLinkedQueue
import kotlin.math.roundToInt


private const val TAG = "SanboxAppFragment"

class SandboxAppFragment : AbstractAppFragment(TAG), SelectColorDialogFragment.SelectColorDialogListener {

    private lateinit var fieldImageView: FieldView
    private lateinit var savePictureButton: Button
    private lateinit var selectColorButton: Button
    private lateinit var clearScreenButton: Button

    enum class ColorPurpose {
        DRAW, CLEAR
    }

    @Volatile
    private var colorPurpose: ColorPurpose = ColorPurpose.DRAW

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_sandbox_app, container, false)
        view?.apply {
            fieldImageView = findViewById(R.id.fieldImageView)
            fieldImageView.drawable.isFilterBitmap = false
            selectColorButton = findViewById<Button>(R.id.sandboxSelectColorButton).apply {
                setOnClickListener {
                    colorPurpose = ColorPurpose.DRAW
                    val newFragment = SelectColorDialogFragment(this@SandboxAppFragment)
                    fragmentManager?.apply {
                        newFragment.show(this, "selectColor")
                    }

                }
            }
            savePictureButton = findViewById<Button>(R.id.sandboxSavePictureButton).apply {
                setOnClickListener {
                    fieldImageView.savePicture()
                }
            }
            clearScreenButton = findViewById<Button>(R.id.sandboxClearScreenButton).apply {
                setOnClickListener {
                    colorPurpose = ColorPurpose.CLEAR
                    val newFragment = SelectColorDialogFragment(this@SandboxAppFragment)
                    fragmentManager?.apply {
                        newFragment.show(this, "selectColor")
                    }

                }
            }
        }
        return view
    }

    override fun onColorSelected(color: Pair<Int, Int>) {
        when (colorPurpose) {
            ColorPurpose.CLEAR -> {
                fieldImageView.clearScreen(color)
            }
            else -> fieldImageView.currentColor = color
        }
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return when (val command = fieldImageView.nextCommand()) {
            is DrawPointCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.toByte()
                        4 -> command.x.toByte()
                        5 -> command.y.toByte()
                        6 -> command.color.toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            is ClearScreenCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.toByte()
                        4 -> command.color.toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            is SavePictureCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            else -> Commands.App.SANDBOX.frame
        }

    }

}

private const val FIELD_WIDTH = 16
private const val FIELD_HEIGHT = 32

class FieldView(context: Context, attributesSet: AttributeSet) :
    ImageView(context, attributesSet) {

    var currentColor: Pair<Int, Int> = 0xFFFFFFFF.toInt() to 0b00111111

    private val bitmap: Bitmap = Bitmap.createBitmap(FIELD_WIDTH, FIELD_HEIGHT, Bitmap.Config.ARGB_8888).apply {
        setImageBitmap(this)
        eraseColor(Color.BLACK)
    }

    private val inverseMatrix: Matrix = Matrix()
    private val points = FloatArray(2)

    private val commandQueue: Queue<Command> = ConcurrentLinkedQueue()
    init {
        commandQueue.add(ClearScreenCommand(0))
    }

    fun nextCommand(): Command? {
        return commandQueue.poll()
    }

    fun savePicture() {
        commandQueue.add(SavePictureCommand())
    }

    fun clearScreen(color: Pair<Int, Int>) {
        commandQueue.add(ClearScreenCommand(color.second))
        bitmap.eraseColor(color.first)
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

        if (y in 0 until FIELD_HEIGHT && x in 0 until FIELD_WIDTH) {
            val command = DrawPointCommand(y, FIELD_WIDTH - x - 1, currentColor.second, bitmap.getPixel(x, y))
            if (command != commandQueue.peek()) {
                commandQueue.add(command)
                bitmap.setPixel(x, y, currentColor.first)
                postInvalidate()
            }
        }
    }

}

sealed class Command(val code: Char)

class DrawPointCommand(val x: Int, val y: Int, val color: Int, val previousColor: Int) :
    Command('P') {

    override fun equals(other: Any?): Boolean {
        return other != null && other is DrawPointCommand &&
                x == other.x && y == other.y && color == other.color
    }

    override fun hashCode(): Int {
        return x + y + color
    }

}

class ClearScreenCommand(val color: Int) : Command('C') {

    override fun equals(other: Any?): Boolean {
        return other != null && other is ClearScreenCommand && color == other.color
    }

    override fun hashCode(): Int {
        return color
    }

}

class SavePictureCommand : Command('S') {

    override fun equals(other: Any?): Boolean {
        return other != null && other is SavePictureCommand
    }

    override fun hashCode(): Int {
        return 0
    }

}