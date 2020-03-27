package org.dragberry.ledecorator.apps.snake

import android.content.Context
import android.net.Uri
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import org.dragberry.ledecorator.R
import java.nio.charset.StandardCharsets

private const val TAG = "SnakeGameFragment"

class SnakeGameFragment : Fragment() {
    private var listener: OnFragmentInteractionListener? = null

    private var leftButton: Button? = null
    private var rightButton: Button? = null
    private var okButton: Button? = null

    enum class Action {
        NO_ACTION, TURN_LEFT, TURN_RIGHT, RESET
    }

    @Volatile
    private var action = Action.NO_ACTION

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.i(TAG, "onCreate")
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        Log.i(TAG, "onCreateView")
        val view = inflater.inflate(R.layout.fragment_snake_game, container, false)
        leftButton = view.findViewById<Button>(R.id.leftButton).apply {
            setOnClickListener {
                Log.i(TAG, "LEFT")
                action = Action.TURN_LEFT
            }
        }
        rightButton = view.findViewById<Button>(R.id.rightButton).apply {
            setOnClickListener {
                Log.i(TAG, "RIGHT")
                action = Action.TURN_RIGHT
            }
        }
//        okButton = view.findViewById<Button>(R.id.okButton).apply {
//            setOnClickListener {
//                Log.i(TAG, "OK")
//                action = Action.RESET
//            }
//        }
        return view
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is OnFragmentInteractionListener) {
            listener = context
        } else {
        }
    }

    override fun onDetach() {
        super.onDetach()
        listener = null
    }

    fun onBleFrameReceived(frame: ByteArray): ByteArray {
        val response =  when (action) {
            Action.TURN_RIGHT -> {
                "11b00000000000000000"
            }
            Action.TURN_LEFT -> {
                "11a00000000000000000"
            }
            Action.RESET -> {
                "10000000000000000000"
            }
            else ->
                "11000000000000000000"
        }.toByteArray(StandardCharsets.US_ASCII)
        action = Action.NO_ACTION
        return response
    }

    interface OnFragmentInteractionListener {
        fun onFragmentInteraction(uri: Uri)
    }

    companion object {

        @JvmStatic
        fun newInstance() = SnakeGameFragment()
    }
}
