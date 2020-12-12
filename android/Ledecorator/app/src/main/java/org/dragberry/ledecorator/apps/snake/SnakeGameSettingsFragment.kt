package org.dragberry.ledecorator.apps.snake


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button

import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands

private const val TAG = "SnakeGameSettingsFragment"

class SnakeGameSettingsFragment :  AbstractAppFragment(TAG) {

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

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view =  inflater.inflate(R.layout.fragment_snake_game_settings, container, false)
        view?.apply {
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

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return SnakeGameFragment.SNAKE_IDLE
    }
}
