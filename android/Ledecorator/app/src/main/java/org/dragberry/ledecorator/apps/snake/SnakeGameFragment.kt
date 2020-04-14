package org.dragberry.ledecorator.apps.snake

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Switch
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.APP_SNAKE
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.COMMAND_INFINITE
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_END
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_START

private const val TAG = "SnakeGameFragment"

class SnakeGameFragment : AbstractAppFragment() {

    override val fragmentId: String = TAG

    private var leftButton: Button? = null
    private var rightButton: Button? = null
    private var autoManualSwitch: Switch? = null

    enum class Action(val value: Byte) {
        NO_ACTION('N'.toByte()),
        TURN_LEFT('L'.toByte()),
        TURN_RIGHT('R'.toByte())
    }

    @Volatile
    private var action = Action.NO_ACTION

    enum class Mode(val value: Byte) {
        AUTO('A'.toByte()),
        MANUAL('M'.toByte())
    }

    @Volatile
    private var mode = Mode.AUTO

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_snake_game, container, false)
        return view?.apply {
            leftButton = findViewById<Button>(R.id.leftButton).apply {
                setOnClickListener {
                    action = Action.TURN_LEFT
                }
            }
            rightButton = findViewById<Button>(R.id.rightButton).apply {
                setOnClickListener {
                    action = Action.TURN_RIGHT
                }
            }
            autoManualSwitch = findViewById<Switch>(R.id.snakeAutoManualModeSwitch).apply {
                text = getString(R.string.ledecorator_app_mode_auto)
                setOnCheckedChangeListener { _, state ->
                    if (state) {
                        mode = Mode.MANUAL
                        text = getString(R.string.ledecorator_app_mode_manual)
                    } else {
                        mode = Mode.AUTO
                        text = getString(R.string.ledecorator_app_mode_auto)
                    }
                }
            }
        }
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return if (mode == Mode.MANUAL) {
            ByteArray(20) {
                when (it) {
                    0 -> FRAME_START
                    1 -> APP_SNAKE
                    2 -> COMMAND_INFINITE
                    3 -> mode.value
                    4 -> action.value
                    19 -> FRAME_END
                    else -> 0
                }
            }.apply {
                action = Action.NO_ACTION
            }
        } else {
            SNAKE_IDLE
        }
    }

    companion object {
        @JvmStatic
        private val SNAKE_IDLE = ByteArray(20) {
            when (it) {
                0 -> FRAME_START
                1 -> APP_SNAKE
                2 -> COMMAND_INFINITE
                3 -> Mode.AUTO.value
                19 -> FRAME_END
                else -> 0
            }
        }
    }
}