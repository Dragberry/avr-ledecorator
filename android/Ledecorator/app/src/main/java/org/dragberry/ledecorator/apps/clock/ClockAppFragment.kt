package org.dragberry.ledecorator.apps.clock

import android.os.Bundle
import android.os.Message
import android.text.Editable
import android.text.TextWatcher
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.BleUtils
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.utils.Colors
import org.dragberry.ledecorator.utils.SelectColorDialogFragment
import java.time.LocalDateTime

private const val TAG = "ClockAppFragment"

private const val LOAD_COMPLETE = 10001

class ClockAppFragment : AbstractAppFragment(TAG) {

    private lateinit var timeColorButton: Button
    private lateinit var secondsColorButton: Button
    private lateinit var dateColorButton: Button
    private lateinit var backgroundColorButton: Button
    private lateinit var timeToLiveEditText: EditText

    private lateinit var saveButton: Button
    private lateinit var setTimeButton: Button
    private lateinit var loadButton: Button

    enum class Action {
        IDLE, SAVE, SET_TIME, LOAD
    }

    @Volatile
    private var action: Action = Action.IDLE

    @Volatile
    private var timeColor = Colors.BLACK

    @Volatile
    private var secondsColor = Colors.BLACK

    @Volatile
    private var dateColor = Colors.BLACK

    @Volatile
    private var backgroundColor = Colors.BLACK

    @Volatile
    private var timeToLive = 5

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_clock_app, container, false)
        view?.apply {
            timeColorButton = findViewById<Button>(R.id.clockAppTimeColorButton).apply {
                setBackgroundColor(timeColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            timeColor = it
                            timeColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            secondsColorButton = findViewById<Button>(R.id.clockAppSecondsColorButton).apply {
                setBackgroundColor(secondsColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            secondsColor = it
                            secondsColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            dateColorButton = findViewById<Button>(R.id.clockAppDateColorButton).apply {
                setBackgroundColor(dateColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            dateColor = it
                            dateColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            backgroundColorButton = findViewById<Button>(R.id.clockAppBackgroundColorButton).apply {
                setBackgroundColor(backgroundColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            backgroundColor = it
                            backgroundColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            timeToLiveEditText = findViewById<EditText>(R.id.clockAppTimeToLiveEditView).apply {
                addTextChangedListener(object : TextWatcher {
                    override fun afterTextChanged(s: Editable?) {}

                    override fun beforeTextChanged(
                        s: CharSequence?,
                        start: Int,
                        count: Int,
                        after: Int
                    ) {
                    }

                    override fun onTextChanged(
                        text: CharSequence?,
                        start: Int,
                        before: Int,
                        count: Int
                    ) {
                        timeToLive = try {
                            text.toString().toInt()
                        } catch (nfe :NumberFormatException) {
                            5
                        }
                    }
                })
            }

            saveButton = findViewById<Button>(R.id.clockAppSaveButton).apply {
                setOnClickListener {
                    action = Action.SAVE
                }
            }
            setTimeButton = findViewById<Button>(R.id.clockAppSetTimeButton).apply {
                setOnClickListener {
                    action = Action.SET_TIME
                }
            }
            loadButton = findViewById<Button>(R.id.clockAppLoadButton).apply {
                setOnClickListener {
                    action = Action.LOAD
                }
            }
        }
        action = Action.LOAD
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            LOAD_COMPLETE -> {
                timeToLiveEditText.setText(timeToLive.toString())
                timeColorButton.setBackgroundColor(timeColor.real)
                secondsColorButton.setBackgroundColor(secondsColor.real)
                dateColorButton.setBackgroundColor(dateColor.real)
                backgroundColorButton.setBackgroundColor(backgroundColor.real)
            }
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (bytes[4] == Commands.System.LOAD.code) {
            timeToLive = BleUtils.uint16(bytes[5], bytes[6])
            timeColor = Colors.getByDisplay(bytes[7].toInt())
            secondsColor = Colors.getByDisplay(bytes[8].toInt())
            dateColor = Colors.getByDisplay(bytes[9].toInt())
            backgroundColor = Colors.getByDisplay(bytes[10].toInt())
            handler?.apply {
                obtainMessage(LOAD_COMPLETE).sendToTarget()
            }
        }
        return when(action) {
            Action.SAVE -> {
                ByteArray(20) {
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.CLOCK.code
                        2 -> Commands.System.INFINITE.code
                        3 -> Commands.System.SAVE.code
                        4 -> BleUtils.byte0(timeToLive)
                        5 -> BleUtils.byte1(timeToLive)
                        6 -> timeColor.display.toByte()
                        7 -> secondsColor.display.toByte()
                        8 -> dateColor.display.toByte()
                        9 -> backgroundColor.display.toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            Action.SET_TIME -> {
                ByteArray(20) {
                    val datetime = LocalDateTime.now()
                    when (it) {
                        0 -> Commands.Frame.START.code
                        1 -> Commands.App.CLOCK.code
                        2 -> Commands.System.INFINITE.code
                        3 -> CLOCK_UPDATE
                        4 -> datetime.hour.toByte()
                        5 -> datetime.minute.toByte()
                        6 -> datetime.second.toByte()
                        7 -> datetime.dayOfMonth.toByte()
                        8 -> datetime.monthValue.toByte()
                        9 -> (datetime.year % 100).toByte()
                        19 -> Commands.Frame.END.code
                        else -> 0
                    }
                }
            }
            Action.LOAD -> CLOCK_LOAD
            else -> Commands.App.CLOCK.frame
        }.also {
            action = Action.IDLE
        }
    }

    companion object {
        private const val CLOCK_UPDATE = 'U'.toByte()

        @JvmStatic
        private val CLOCK_LOAD = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.CLOCK.code
                2 -> Commands.System.INFINITE.code
                3 -> Commands.System.LOAD.code
                19 -> Commands.Frame.END.code
                else -> 0
            }
        }
    }
}