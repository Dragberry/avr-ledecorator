package org.dragberry.ledecorator.apps.life


import android.os.Bundle
import android.os.Message
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
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

    enum class Action(val value: Byte) {
        IDLE('I'.toByte()),
        SAVE('S'.toByte()),
        RESTART(Commands.System.RESTART.code),
        LOAD('L'.toByte())
    }

    enum class Mode(val value: Byte) {
        RANDOM(0.toByte()),
        CAROUSEL(1.toByte()),
        CONSTANT(2.toByte())
    }

    enum class Script(val value: Byte) {
        RANDOM_SEA(0.toByte()),
        SHIPS(1.toByte()),
        SHIPS_RANDOM(2.toByte()),
        COPERHEAD(3.toByte()),
    }

    @Volatile
    private var action = Action.LOAD

    @Volatile
    private var liveColor = Colors.WHITE

    @Volatile
    private var deadColor = Colors.BLACK

    @Volatile
    private var mode = Mode.CAROUSEL

    @Volatile
    private var script = Script.COPERHEAD

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

            }
            speedSelectorSpinner = findViewById<Spinner>(R.id.lifeGameSpeedSelector).apply {
                adapter = ArrayAdapter<Int>(
                    context, R.layout.fragment_spinner_text_item, Array(10) { it }
                )
            }


            saveButton = findViewById<Button>(R.id.lifeGameSaveButton).apply {
                setOnClickListener {
                    action = Action.SAVE
                }
            }
            restartButton = findViewById<Button>(R.id.lifeGameRestartButton).apply {
                setOnClickListener {
                    action = Action.RESTART
                }
            }
            loadButton = findViewById<Button>(R.id.lifeGameLoadButton).apply {
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
                liveColorButton.setBackgroundColor(liveColor.real)
                deadColorButton.setBackgroundColor(deadColor.real)
                modeSelectorSpinner.setSelection(mode.ordinal)
                scriptSelectorSpinner.setSelection(script.ordinal)
            }
        }
        return true
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        if (bytes[6] == Action.LOAD.value) {
            liveColor = Colors.getByDisplay(bytes[7].toInt())
            deadColor = Colors.getByDisplay(bytes[8].toInt())
            mode = Mode.values().first { it.value == bytes[9] }
            script = Script.values().first { it.value == bytes[10] }
            handler?.apply {
                obtainMessage(LOAD_COMPLETE).sendToTarget()
            }
            action = Action.IDLE
            return Commands.App.LIFE.frame
        }
        return when (action) {
            Action.RESTART -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.LIFE.code
                    2 -> Commands.System.RESTART.code
                    19 -> Commands.Frame.END.code
                    else -> 0
                }.apply {
                    action = Action.IDLE
                }
            }
            Action.SAVE -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.LIFE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> Action.SAVE.value
                    4 -> liveColor.display.toByte()
                    5 -> deadColor.display.toByte()
                    6 -> mode.value
                    7 -> script.value
                    19 -> Commands.Frame.END.code
                    else -> 0
                }.apply {
                    action = Action.IDLE
                }
            }
            Action.LOAD -> ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.LIFE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> Action.LOAD.value
                    19 -> Commands.Frame.END.code
                    else -> 0
                }.apply {
                    action = Action.IDLE
                }
            }
            else -> Commands.App.LIFE.frame
        }
    }


}
