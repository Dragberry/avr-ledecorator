package org.dragberry.ledecorator.apps.snake


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button

import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands

private const val TAG = "SnakeGameManualFragment"

class SnakeGameManualFragment : AbstractAppFragment(TAG) {

    enum class Action(val value: Byte) {
        NO_ACTION('N'.toByte()),
        TURN_LEFT('L'.toByte()),
        TURN_RIGHT('R'.toByte())
    }

    private lateinit var leftButton: Button
    private lateinit var rightButton: Button

    @Volatile
    private var action = Action.NO_ACTION

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view =  inflater.inflate(R.layout.fragment_snake_game_manual, container, false)
        return view?.apply {
            leftButton = findViewById<Button>(R.id.snakeGameManualLeftButton).apply {
                setOnClickListener {
                    action = Action.TURN_LEFT
                }
            }
            rightButton = findViewById<Button>(R.id.snakeGameManualRightButton).apply {
                setOnClickListener {
                    action = Action.TURN_RIGHT
                }
            }
        }

    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return if (action != Action.NO_ACTION) {
            ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.SNAKE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> SnakeGameFragment.Mode.MANUAL.value
                    4 -> action.value
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
        } else {
            ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.SNAKE.code
                    2 -> Commands.System.INFINITE.code
                    3 -> SnakeGameFragment.Mode.MANUAL.value
                    4 -> Action.NO_ACTION.value
                    19 -> Commands.Frame.END.code
                    else -> 0
                }
            }
        }.apply {
            action = Action.NO_ACTION
        }
    }

}
