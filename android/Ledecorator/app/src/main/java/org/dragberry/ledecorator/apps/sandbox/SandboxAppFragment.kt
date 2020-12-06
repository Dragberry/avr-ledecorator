package org.dragberry.ledecorator.apps.sandbox

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
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
import java.io.IOException
import java.util.*
import java.util.concurrent.ConcurrentLinkedQueue


private const val TAG = "SanboxAppFragment"

private const val ENABLE_BUTTONS = 6000
private const val UPDATE_FIELD = 6001
private const val SAVE_FILE = 6002
private const val OPEN_FILE = 6003

class SandboxAppFragment : AbstractAppFragment(TAG) {

    private lateinit var fieldImageView: FieldView
    private lateinit var savePictureButton: Button
    private lateinit var openPictureButton: Button
    private lateinit var selectColorButton: Button
    private lateinit var clearScreenButton: Button
    private lateinit var loadPictureButton: Button
    private lateinit var writeButton: Button

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
            writeButton = findViewById<Button>(R.id.sandboxWriteButton).apply {
                setOnClickListener { commandQueue.add(SavePictureCommand()) }
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
                    val intent = Intent(Intent.ACTION_CREATE_DOCUMENT)
                    intent.addCategory(Intent.CATEGORY_OPENABLE)
                    intent.type = "image/png"
                    startActivityForResult(intent, SAVE_FILE)
                }
            }
            openPictureButton = findViewById<Button>(R.id.sandboxOpenPictureButton).apply {
                setOnClickListener {
                    val intent = Intent(Intent.ACTION_OPEN_DOCUMENT)
                    intent.addCategory(Intent.CATEGORY_OPENABLE)
                    intent.type = "image/*"
                    startActivityForResult(intent, OPEN_FILE)
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
        startLoading()
        return view
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                SAVE_FILE -> {
                    data?.data?.apply {
                        try {
                            val output = context!!.contentResolver.openOutputStream(this)
                            fieldImageView.getImage {
                                it.compress(Bitmap.CompressFormat.PNG, 100, output)
                            }
                            output!!.flush()
                            output.close()
                        } catch (exc: IOException) {
                            Log.e(TAG, "Failed to save image", exc)
                        }
                    }
                }
                OPEN_FILE -> {
                    try {
                        data?.data?.apply {
                            val input = context!!.contentResolver.openInputStream(this)
                            fieldImageView.setImage(input!!) { x, y, size, block ->
                                commandQueue.add(DrawBlockCommand(x, y, size, block))
                            }
                            input.close()
                        }
                    } catch (exc: IOException) {
                        Log.e(TAG, "Failed to save image", exc)
                    }
                }
            }
        }
    }

    private fun startLoading() {
        loadingInProgress = true
        selectColorButton.isEnabled = false
        savePictureButton.isEnabled = false
        clearScreenButton.isEnabled = false
        openPictureButton.isEnabled = false
        writeButton.isEnabled = false
        loadPictureButton.text = getString(R.string.loading_in_progress)
        commandQueue.add(LoadPictureCommand(0, 0))
    }


    private fun stopLoading() {
        loadingInProgress = false
        selectColorButton.isEnabled = true
        savePictureButton.isEnabled = true
        clearScreenButton.isEnabled = true
        openPictureButton.isEnabled = true
        writeButton.isEnabled = true
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
            is DrawBlockCommand -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.SANDBOX.code
                        2 -> Commands.System.INFINITE.code
                        3 -> command.code.value
                        4 -> command.x.toByte()
                        5 -> command.y.toByte()
                        6 -> command.size.toByte()
                        in 7..18 -> command.block[it - 7].toByte()
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
