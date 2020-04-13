package org.dragberry.ledecorator.apps.clock

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.MainActivity
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.APP_CLOCK
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.COMMAND_INFINITE
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_END
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_START
import java.time.LocalDateTime

private const val TAG = "ClockAppFragment"

class ClockAppFragment : Fragment() {

    private var bluetoothService: MainActivity.BluetoothService? = null

    private var updateButton: Button? = null

    @Volatile
    private var updateRequested: Boolean = false

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_clock_app, container, false)
        return view?.apply {
            updateButton = findViewById<Button>(R.id.clockAppUpdateButton)?.apply {
                setOnClickListener {
                    updateRequested = true
                }
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(TAG) {
                if (updateRequested)
                {
                    bluetoothService?.responseDataFrame = ByteArray(20) {
                        val datetime = LocalDateTime.now()
                        when (it) {
                            0 -> FRAME_START
                            1 -> APP_CLOCK
                            2 -> COMMAND_INFINITE
                            3 -> 'U'.toByte()
                            4 -> datetime.hour.toByte()
                            5 -> datetime.minute.toByte()
                            6 -> datetime.second.toByte()
                            7 -> datetime.dayOfMonth.toByte()
                            8 -> datetime.monthValue.toByte()
                            9 -> (datetime.year % 100).toByte()
                            19 -> FRAME_END
                            else -> 0
                        }
                    }
                    updateRequested = false
                }
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