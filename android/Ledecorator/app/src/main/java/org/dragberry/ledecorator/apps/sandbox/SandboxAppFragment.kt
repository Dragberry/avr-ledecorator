package org.dragberry.ledecorator.apps.sandbox

import android.os.Bundle
import android.os.Message
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.utils.Colors
import org.dragberry.ledecorator.utils.SelectColorDialogFragment
import java.util.*
import java.util.concurrent.ConcurrentLinkedQueue


private const val TAG = "SanboxAppFragment"

private const val ENABLE_BUTTONS = 6000
private const val UPDATE_FIELD = 6001

class SandboxAppFragment : AbstractAppFragment(TAG) {

    private lateinit var fieldImageView: FieldView
    private lateinit var savePictureButton: Button
    private lateinit var selectColorButton: Button
    private lateinit var clearScreenButton: Button
    private lateinit var loadPictureButton: Button

    private val commandQueue: Queue<Command> = ConcurrentLinkedQueue()

    private var loadingInProgress: Boolean = false

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_sandbox_app, container, false)
        view?.apply {
            fieldImageView = findViewById<FieldView>(R.id.sandboxFieldImageView).apply {
                drawable.isFilterBitmap = false
                onDrawPoint { x, y, color ->
                    commandQueue.add(DrawPointCommand(x, y, color.display, color.display))
                }
            }
            selectColorButton = findViewById<Button>(R.id.sandboxSelectColorButton).apply {
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            selectColorButton.setBackgroundColor(it.real)
                            fieldImageView.currentColor = it
                        }.show(this, "selectColor")
                    }
                }
            }
            savePictureButton = findViewById<Button>(R.id.sandboxSavePictureButton).apply {
                setOnClickListener {
                    commandQueue.add(SavePictureCommand())
                }
            }
            clearScreenButton = findViewById<Button>(R.id.sandboxClearScreenButton).apply {
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            clearScreenButton.setBackgroundColor(it.real)
                            commandQueue.add(ClearScreenCommand(it.display))
                            fieldImageView.clearScreen(it)
                        }.show(this, "selectColor")
                    }
                }
            }
            loadPictureButton = findViewById<Button>(R.id.sandboxLoadPictureButton).apply {
                setOnClickListener {
                    if (loadingInProgress) {
                        stopLoading()
                    } else {
                        startLoading()
                    }
                }
            }
        }
        return view
    }

    private fun startLoading() {
        loadingInProgress = true
        selectColorButton.isEnabled = false
        savePictureButton.isEnabled = false
        clearScreenButton.isEnabled = false
        loadPictureButton.text = getString(R.string.loading_in_progress)
        commandQueue.add(LoadPictureCommand(0, 0))
    }


    private fun stopLoading() {
        loadingInProgress = false
        selectColorButton.isEnabled = true
        savePictureButton.isEnabled = true
        clearScreenButton.isEnabled = true
        loadPictureButton.text = getString(R.string.load)
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            ENABLE_BUTTONS -> stopLoading()
            UPDATE_FIELD -> fieldImageView.postInvalidate()
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (loadingInProgress && bytes[4] == Action.LOAD_PICTURE.value) {
            var x: Int = bytes[5].toInt()
            var y: Int = bytes[6].toInt()
            Log.i(TAG, "X=$x Y=$y   $bytes")
            for (i in 8..15) {
                fieldImageView.setPixel(x, y, Colors.convertColor(bytes[i].toInt()))
                if (++x == FIELD_WIDTH) {
                    x = 0
                    if (++y == FIELD_HEIGHT) {
                        y = 0
                        break
                    }
                }
            }
            handler?.apply {
                obtainMessage(UPDATE_FIELD).sendToTarget()
            }
            if (x == 0 && y == 0) {
                handler?.apply {
                    obtainMessage(ENABLE_BUTTONS).sendToTarget()
                }
                return Commands.App.SANDBOX.frame
            }
            commandQueue.add(LoadPictureCommand(x, y))
        }
        return when (val command = commandQueue.poll()) {
            is DrawPointCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.value
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
                        3 -> command.code.value
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
                        3 -> command.code.value
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            is LoadPictureCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.value
                        4 -> command.x.toByte()
                        5 -> command.y.toByte()
                        6 -> command.size.toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            else -> Commands.App.SANDBOX.frame
        }
    }
}
