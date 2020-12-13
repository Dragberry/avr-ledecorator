package org.dragberry.ledecorator.apps.life


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


private const val TAG = "LifeGameFragment"

private const val LOAD_COMPLETE = 7000

class LifeGameFragment : AbstractAppFragment(TAG) {

    private lateinit var liveColorButton: Button
    private lateinit var deadColorButton: Button
    private lateinit var modeSelectorSpinner: Spinner
    private lateinit var scriptSelectorSpinner: Spinner
    private lateinit var timeToLiveEditText: EditText
    private lateinit var speedSelectorSpinner: Spinner

    private lateinit var saveButton: Button
    private lateinit var restartButton: Button
    private lateinit var loadButton: Button

    enum class Mode(val value: Byte) {
        RANDOM(0.toByte()),
        CAROUSEL(1.toByte()),
        CONSTANT(2.toByte());

        companion object {
            fun valueOf(code: Byte): Mode = values().find { it.value == code } ?: RANDOM
        }
    }

    enum class Script(val value: Byte) {
        RANDOM_SEA(0.toByte()),
        SHIPS(1.toByte()),
        SHIPS_RANDOM(2.toByte()),
        COPERHEAD(3.toByte());

        companion object {
            fun valueOf(code: Byte): Script = values().find { it.value == code } ?: RANDOM_SEA
        }
    }

    @Volatile
    private var action = Commands.System.EMPTY

    @Volatile
    private var liveColor = Colors.WHITE

    @Volatile
    private var deadColor = Colors.BLACK

    @Volatile
    private var mode = Mode.CAROUSEL

    @Volatile
    private var script = Script.COPERHEAD

    @Volatile
    private var timeToLive = 5

    @Volatile
    private var speed = 1

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_life_game, container, false)?.apply {
            liveColorButton = findViewById<Button>(R.id.lifeGameLiveColorButton).apply {
                setBackgroundColor(liveColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            liveColor = it
                            liveColorButton.setBackgroundColor(it.real)
                        }.show(this, "selectColor")
                    }
                }
            }
            deadColorButton = findViewById<Button>(R.id.lifeGameDeadColorButton).apply {
                setBackgroundColor(deadColor.real)
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            deadColor = it
                            deadColorButton.setBackgroundColor(it.real)
                        }.show(this, "selectColor")
                    }
                }
            }
            modeSelectorSpinner = findViewById<Spinner>(R.id.lifeGameModeSelector).apply {
                adapter = ArrayAdapter<Mode>(context, R.layout.fragment_spinner_text_item, Mode.values())
                onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                    override fun onNothingSelected(parent: AdapterView<*>?) {}
                    override fun onItemSelected(
                        parent: AdapterView<*>?,
                        view: View?,
                        position: Int,
                        id: Long
                    ) {
                        mode = Mode.values()[position]
                    }

                }
            }
            scriptSelectorSpinner = findViewById<Spinner>(R.id.lifeGameScriptSelector).apply {
                adapter = ArrayAdapter<Script>(context, R.layout.fragment_spinner_text_item, Script.values())
                onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                    override fun onNothingSelected(parent: AdapterView<*>?) {}
                    override fun onItemSelected(
                        parent: AdapterView<*>?,
                        view: View?,
                        position: Int,
                        id: Long
                    ) {
                        script = Script.values()[position]
                    }

                }
            }
            timeToLiveEditText = findViewById<EditText>(R.id.lifeGameTimeToLiveEditView).apply {
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
            speedSelectorSpinner = findViewById<Spinner>(R.id.lifeGameSpeedSelector).apply {
                adapter = ArrayAdapter<Int>(
                    context, R.layout.fragment_spinner_text_item, Array(10) { it + 1 }
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


            saveButton = findViewById<Button>(R.id.lifeGameSaveButton).apply {
                setOnClickListener {
                    action = Commands.System.SAVE
                }
            }
            restartButton = findViewById<Button>(R.id.lifeGameRestartButton).apply {
                setOnClickListener {
                    action = Commands.System.RESTART
                }
            }
            loadButton = findViewById<Button>(R.id.lifeGameLoadButton).apply {
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
                liveColorButton.setBackgroundColor(liveColor.real)
                deadColorButton.setBackgroundColor(deadColor.real)
                modeSelectorSpinner.setSelection(mode.ordinal)
                scriptSelectorSpinner.setSelection(script.ordinal)
                timeToLiveEditText.setText(timeToLive.toString())
                speedSelectorSpinner.setSelection(speed - 1)
            }
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (bytes[4] == Commands.System.LOAD.code) {
            timeToLive = BleUtils.uint16(bytes[5], bytes[6])
            liveColor = Colors.getByDisplay(bytes[7].toInt())
            deadColor = Colors.getByDisplay(bytes[8].toInt())
            mode = Mode.valueOf(bytes[9])
            script = Script.valueOf(bytes[10])
            speed = bytes[11].toInt()
            action = Commands.System.EMPTY
            handler?.apply {
                obtainMessage(LOAD_COMPLETE).sendToTarget()
            }
        }
        return when (action) {
            Commands.System.RESTART -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.LIFE.code
                    2 -> Commands.System.RESTART.code
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
            Commands.System.SAVE -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.LIFE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> Commands.System.SAVE.code
                    4 -> BleUtils.byte0(timeToLive)
                    5 -> BleUtils.byte1(timeToLive)
                    6 -> liveColor.display.toByte()
                    7 -> deadColor.display.toByte()
                    8 -> mode.value
                    9 -> script.value
                    10 -> speed.toByte()
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
            Commands.System.LOAD -> LIFE_LOAD
            else -> Commands.App.LIFE.frame
        }.also {
            action = Commands.System.EMPTY
        }
    }

    companion object {
        @JvmStatic
        val LIFE_LOAD = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.LIFE.code
                2 -> Commands.System.INFINITE.code
                3 -> Commands.System.LOAD.code
                19 -> Commands.Frame.END.code
                else -> 0
            }
        }
    }
}
