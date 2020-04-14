package org.dragberry.ledecorator.apps.clock

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.APP_CLOCK
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.COMMAND_INFINITE
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_END
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame.Companion.FRAME_START
import java.time.LocalDateTime

private const val TAG = "ClockAppFragment"

class ClockAppFragment : AbstractAppFragment() {

    override val fragmentId: String = TAG

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

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return if (updateRequested)
        {
            updateRequested = false
            ByteArray(20) {
                val datetime = LocalDateTime.now()
                when (it) {
                    0 -> FRAME_START
                    1 -> APP_CLOCK
                    2 -> COMMAND_INFINITE
                    3 -> CLOCK_UPDATE
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
        } else {
            CLOCK_IDLE
        }
    }

    companion object {
        private const val CLOCK_UPDATE = 'U'.toByte()

        @JvmStatic
        private val CLOCK_IDLE = ByteArray(20) {
            when (it) {
                0 -> FRAME_START
                1 -> APP_CLOCK
                2 -> COMMAND_INFINITE
                19 -> FRAME_END
                else -> 0
            }
        }
    }
}