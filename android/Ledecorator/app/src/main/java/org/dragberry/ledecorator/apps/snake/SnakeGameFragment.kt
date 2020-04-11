package org.dragberry.ledecorator.apps.snake

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.MainActivity
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.APP_SNAKE
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_END
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_START

private const val TAG = "SnakeGameFragment"

class SnakeGameFragment : Fragment() {

    private var bluetoothService: MainActivity.BluetoothService? = null

    private var leftButton: Button? = null
    private var rightButton: Button? = null

    enum class Action {
        NO_ACTION, TURN_LEFT, TURN_RIGHT
    }

    @Volatile
    private var action = Action.NO_ACTION

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_snake_game, container, false)
        leftButton = view.findViewById<Button>(R.id.leftButton).apply {
            setOnClickListener {
                action = Action.TURN_LEFT
            }
        }
        rightButton = view.findViewById<Button>(R.id.rightButton).apply {
            setOnClickListener {
                action = Action.TURN_RIGHT
            }
        }
        return view
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(TAG) {
                bluetoothService?.responseDataFrame = ByteArray(20) {
                    when (it) {
                        0 -> FRAME_START
                        1 -> APP_SNAKE
                        2 -> {
                            when (action) {
                                Action.TURN_LEFT -> 'L'
                                Action.TURN_RIGHT -> 'R'
                                else -> 'I'
                            }.toByte()
                        }
                        19 -> FRAME_END
                        else -> 0
                    }
                }
                action = Action.NO_ACTION
            }
        } else {
            throw RuntimeException("$context must implement ${BluetoothServiceHolder::javaClass.name}")
        }
    }

    override fun onDetach() {
        super.onDetach()
        bluetoothService?.onDataFrame(TAG, null)
        bluetoothService = null
    }
}