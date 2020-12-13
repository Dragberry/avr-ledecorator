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
    private lateinit var foodIncrementColorButton: Button
    private lateinit var foodDecrementColorButton: Button
    private lateinit var foodSpeedUpColorButton: Button
    private lateinit var foodSpeedDownColorButton: Button
    private lateinit var wallColorButton: Button
    private lateinit var wallSelectorSpinner: Spinner
    private lateinit var timeToLiveEditText: EditText
    private lateinit var speedSelectorSpinner: Spinner

    private lateinit var saveButton: Button
    private lateinit var restartButton: Button
    private lateinit var loadButton: Button

    enum class Wall(val value: Byte) {
        NO(0.toByte()),
        CROSS(1.toByte()),
        TUNNEL(2.toByte());

        companion object {
            fun valueOf(code: Byte): Wall = values().find { it.value == code } ?: NO
        }
    }

    @Volatile
    private var action = Commands.System.LOAD

    @Volatile
    private var fieldColor = Colors.BLACK

    @Volatile
    private var headColor = Colors.BLACK

    @Volatile
    private var bodyColor = Colors.BLACK

    @Volatile
    private var deadColor = Colors.BLACK

    @Volatile
    private var foodIncrementColor = Colors.BLACK

    @Volatile
    private var foodDecrementColor = Colors.BLACK

    @Volatile
    private var foodSpeedUpColor = Colors.BLACK

    @Volatile
    private var foodSpeedDownColor = Colors.BLACK

    @Volatile
    private var wallColor = Colors.BLACK

    @Volatile
    private var wall = Wall.NO

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
            foodIncrementColorButton = findViewById<Button>(R.id.snakeGameSettingsFoodIncrementColorButton).apply {
                setBackgroundColor(foodIncrementColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            foodIncrementColor = it
                            foodIncrementColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            foodDecrementColorButton = findViewById<Button>(R.id.snakeGameSettingsFoodDecrementColorButton).apply {
                setBackgroundColor(foodDecrementColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            foodDecrementColor = it
                            foodDecrementColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            foodSpeedUpColorButton = findViewById<Button>(R.id.snakeGameSettingsFoodSpeedUpColorButton).apply {
                setBackgroundColor(foodSpeedUpColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            foodSpeedUpColor = it
                            foodSpeedUpColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            foodSpeedDownColorButton = findViewById<Button>(R.id.snakeGameSettingsFoodSpeedDownColorButton).apply {
                setBackgroundColor(foodSpeedDownColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            foodSpeedDownColor = it
                            foodSpeedDownColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            wallColorButton = findViewById<Button>(R.id.snakeGameSettingsWallColorButton).apply {
                setBackgroundColor(wallColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            wallColor = it
                            wallColorButton.setBackgroundColor(it.real)
                        }.show(this, SelectColorDialogFragment.DIALOG_TAG)
                    }
                }
            }
            wallSelectorSpinner = findViewById<Spinner>(R.id.snakeGameSettingsWallSelector).apply {
                adapter = ArrayAdapter<Wall>(context, R.layout.fragment_spinner_text_item, Wall.values())
                onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                    override fun onNothingSelected(parent: AdapterView<*>?) {}
                    override fun onItemSelected(
                        parent: AdapterView<*>?,
                        view: View?,
                        position: Int,
                        id: Long
                    ) {
                        wall = Wall.values()[position]
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
                    action = Commands.System.SAVE
                }
            }
            restartButton = findViewById<Button>(R.id.snakeGameSettingsRestartButton).apply {
                setOnClickListener {
                    action = Commands.System.RESTART
                }
            }
            loadButton = findViewById<Button>(R.id.snakeGameSettingsLoadButton).apply {
                setOnClickListener {
                    action = Commands.System.LOAD
                }
            }
        }
        action = Commands.System.LOAD
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            LOAD_COMPLETE -> {
                timeToLiveEditText.setText(timeToLive.toString())
                fieldColorButton.setBackgroundColor(fieldColor.real)
                headColorButton.setBackgroundColor(headColor.real)
                bodyColorButton.setBackgroundColor(bodyColor.real)
                deadColorButton.setBackgroundColor(deadColor.real)
                foodIncrementColorButton.setBackgroundColor(foodIncrementColor.real)
                foodDecrementColorButton.setBackgroundColor(foodDecrementColor.real)
                foodSpeedUpColorButton.setBackgroundColor(foodSpeedUpColor.real)
                foodSpeedDownColorButton.setBackgroundColor(foodSpeedDownColor.real)
                wallColorButton.setBackgroundColor(wallColor.real)
                wallSelectorSpinner.setSelection(wall.ordinal)
                speedSelectorSpinner.setSelection(speed - 1)
            }
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (bytes[4] == Commands.System.LOAD.code) {
            timeToLive = BleUtils.uint16(bytes[5], bytes[6])
            fieldColor = Colors.getByDisplay(bytes[7].toInt())
            headColor = Colors.getByDisplay(bytes[8].toInt())
            bodyColor = Colors.getByDisplay(bytes[9].toInt())
            deadColor = Colors.getByDisplay(bytes[10].toInt())
            foodIncrementColor = Colors.getByDisplay(bytes[11].toInt())
            foodDecrementColor = Colors.getByDisplay(bytes[12].toInt())
            foodSpeedUpColor = Colors.getByDisplay(bytes[13].toInt())
            foodSpeedDownColor = Colors.getByDisplay(bytes[14].toInt())
            wallColor = Colors.getByDisplay(bytes[15].toInt())
            wall = Wall.valueOf(bytes[16])
            speed = bytes[17].toInt()
            handler?.apply {
                obtainMessage(LOAD_COMPLETE).sendToTarget()
            }
        }
        return when(action) {
            Commands.System.RESTART -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.SNAKE.code
                    2 -> Commands.System.RESTART.code
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
            Commands.System.SAVE -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.SNAKE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> Commands.System.SAVE.code
                    4 -> BleUtils.byte0(timeToLive)
                    5 -> BleUtils.byte1(timeToLive)
                    6 -> fieldColor.display.toByte()
                    7 -> headColor.display.toByte()
                    8 -> bodyColor.display.toByte()
                    9 -> deadColor.display.toByte()
                    10 -> foodIncrementColor.display.toByte()
                    11 -> foodDecrementColor.display.toByte()
                    12 -> foodSpeedUpColor.display.toByte()
                    13 -> foodSpeedDownColor.display.toByte()
                    14 -> wallColor.display.toByte()
                    15 -> wall.value
                    16 -> speed.toByte()
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
            Commands.System.LOAD -> SNAKE_LOAD
            else -> Commands.App.SNAKE.frame
        }.also {
            action = Commands.System.EMPTY
        }
    }

    companion object {
        @JvmStatic
        val SNAKE_LOAD = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.SNAKE.code
                2 -> Commands.System.INFINITE.code
                3 -> Commands.System.LOAD.code
                19 -> Commands.Frame.END.code
                else -> 0
            }
        }
    }
}
