package org.dragberry.ledecorator.apps.snake


import android.os.Bundle
import android.os.Message
import android.text.Editable
import android.text.TextWatcher
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*

import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.BleUtils
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.utils.Colors
import org.dragberry.ledecorator.utils.SelectColorDialogFragment

private const val TAG = "SnakeGameSettingsFragment"

private const val LOAD_COMPLETE = 8001

class SnakeGameSettingsFragment :  AbstractAppFragment(TAG) {

    private lateinit var fieldColorButton: Button
    private lateinit var headColorButton: Button
    private lateinit var bodyColorButton: Button
    private lateinit var deadColorButton: Button
    private lateinit var timeToLiveEditText: EditText
    private lateinit var speedSelectorSpinner: Spinner

    private lateinit var saveButton: Button
    private lateinit var restartButton: Button
    private lateinit var loadButton: Button

    enum class Action(val value: Byte) {
        IDLE('I'.toByte()),
        SAVE('S'.toByte()),
        RESTART(Commands.System.RESTART.code),
        LOAD('L'.toByte())
    }

    @Volatile
    private var action = Action.LOAD

    @Volatile
    private var fieldColor = Colors.BLACK

    @Volatile
    private var headColor = Colors.BLACK

    @Volatile
    private var bodyColor = Colors.BLACK

    @Volatile
    private var deadColor = Colors.BLACK

    @Volatile
    private var timeToLive = 5

    @Volatile
    private var speed = 1

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view =  inflater.inflate(R.layout.fragment_snake_game_settings, container, false)
        view?.apply {
            fieldColorButton = findViewById<Button>(R.id.snakeGameSettingsFieldColorButton).apply {
                setBackgroundColor(fieldColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            fieldColor = it
                            fieldColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            headColorButton = findViewById<Button>(R.id.snakeGameSettingsSnakeHeadColorButton).apply {
                setBackgroundColor(headColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            headColor = it
                            headColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            bodyColorButton = findViewById<Button>(R.id.snakeGameSettingsSnakeBodyColorButton).apply {
                setBackgroundColor(bodyColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            bodyColor = it
                            bodyColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            deadColorButton = findViewById<Button>(R.id.snakeGameSettingsSnakeDeadColorButton).apply {
                setBackgroundColor(deadColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            deadColor = it
                            deadColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            timeToLiveEditText = findViewById<EditText>(R.id.snakeGameSettingsTimeToLiveEditView).apply {
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
            speedSelectorSpinner = findViewById<Spinner>(R.id.snakeGameSettingsSpeedSelector).apply {
                adapter = ArrayAdapter<Int>(
                    context, R.layout.fragment_spinner_text_item, Array(40) { it + 1 }
                )
                onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                    override fun onNothingSelected(parent: AdapterView<*>?) {}
                    override fun onItemSelected(
                        parent: AdapterView<*>?,
                        view: View?,
                        position: Int,
                        id: Long
                    ) {
                        speed = position + 1
                    }
                }
            }

            saveButton = findViewById<Button>(R.id.snakeGameSettingsSaveButton).apply {
                setOnClickListener {
                    action = Action.SAVE
                }
            }
            restartButton = findViewById<Button>(R.id.snakeGameSettingsRestartButton).apply {
                setOnClickListener {
                    action = Action.RESTART
                }
            }
            loadButton = findViewById<Button>(R.id.snakeGameSettingsLoadButton).apply {
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
                fieldColorButton.setBackgroundColor(fieldColor.real)
                headColorButton.setBackgroundColor(headColor.real)
                bodyColorButton.setBackgroundColor(bodyColor.real)
                deadColorButton.setBackgroundColor(deadColor.real)
                speedSelectorSpinner.setSelection(speed - 1)
                timeToLiveEditText.setText(timeToLive.toString())
            }
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (bytes[4] == SnakeGameFragment.Mode.LOAD.value) {
            timeToLive = BleUtils.uint16(bytes[5], bytes[6])
            fieldColor = Colors.getByDisplay(bytes[7].toInt())
            headColor = Colors.getByDisplay(bytes[8].toInt())
            bodyColor = Colors.getByDisplay(bytes[9].toInt())
            deadColor = Colors.getByDisplay(bytes[10].toInt())
            speed = bytes[11].toInt()
            handler?.apply {
                obtainMessage(LOAD_COMPLETE).sendToTarget()
            }
        }
        return when(action) {
            Action.LOAD -> SNAKE_LOAD
            else -> SnakeGameFragment.SNAKE_IDLE
        }.also {
            action = Action.IDLE
        }
    }

    companion object {
        @JvmStatic
        val SNAKE_LOAD = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.SNAKE.code
                2 -> Commands.System.INFINITE.code
                3 -> SnakeGameFragment.Mode.LOAD.value
                19 -> Commands.Frame.END.code
                else -> 0
            }
        }
    }
}
