package org.dragberry.ledecorator.bluetooth.fragment


import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.synthetic.main.fragment_ble_console_record.view.*
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.LEDECORATOR_DATAFRAME_RECIEVED
import org.dragberry.ledecorator.MainActivity
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame
import org.dragberry.ledecorator.bluetooth.BleUtils
import java.nio.charset.StandardCharsets
import java.time.LocalTime
import java.util.*

private const val TAG = "BleConsoleFragment"

private const val BUFFER_SIZE = 10

class BleConsoleFragment : Fragment(), Handler.Callback {

    private var bluetoothService: MainActivity.BluetoothService? = null

    private var consoleBuffer: LinkedList<Record> = LinkedList()

    private var recyclerView: RecyclerView? = null

    private lateinit var consoleRecyclerViewAdapter: ConsoleRecyclerViewAdapter

    private var handler: Handler? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        handler = Handler(this)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(TAG) {
                val data: ByteArray = this

                val string: String = StringBuffer().apply {
                    when (data[1]) {
                        BleInterchangeFrame.APP_SNAKE -> {
                            append("Snake:\n")
                            append("\tHead[X=${data[2]};Y=${data[3]}]\n")
                            append("\tTail[X=${data[4]};Y=${data[5]}]\n")
                            append("\tSpeed[${data[6]}]\n")
                            append("\tTime[${BleUtils.uint16(data[7], data[8])}]")
                        }
                        else -> {
                          append("Unknown")
                        }
                    }
                }.toString()

                consoleBuffer.add(Record(LocalTime.now(), string))
                if (consoleBuffer.size > BUFFER_SIZE) {
                    consoleBuffer.pop()
                }
                handler?.apply {
                    obtainMessage(LEDECORATOR_DATAFRAME_RECIEVED).sendToTarget()
                }
            }
        } else {
            throw RuntimeException("$context must implement BluetoothServiceHolder")
        }
    }

    override fun onDetach() {
        super.onDetach()
        bluetoothService?.onDataFrame(TAG, null)
        bluetoothService = null
        handler?.removeCallbacksAndMessages(null)
        handler = null
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_ble_console, container, false)
        if (view != null) {
            consoleRecyclerViewAdapter = ConsoleRecyclerViewAdapter()
            consoleRecyclerViewAdapter.submitList(consoleBuffer)
            recyclerView = view.findViewById<RecyclerView>(R.id.console_recycler_view)?.apply {
                layoutManager = LinearLayoutManager(context)
                adapter = consoleRecyclerViewAdapter
            }
        }
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            LEDECORATOR_DATAFRAME_RECIEVED -> {
                consoleRecyclerViewAdapter.notifyDataSetChanged()
                recyclerView?.scrollToPosition(consoleBuffer.size - 1)
            }
        }
        return true
    }

    inner class Record(val date: LocalTime, val text: String) {
        val dataText: String = date.toString()
    }

    inner class ConsoleLineViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
        val dateTimeTextView: TextView = view.consoleRecordDateTimeTextView
        val valueTextView: TextView = view.consoleRecordValueTextView
    }

    inner class ConsoleRecyclerViewAdapter : ListAdapter<Record, ConsoleLineViewHolder>(
        object : DiffUtil.ItemCallback<Record>() {
            override fun areItemsTheSame(oldItem: Record, newItem: Record): Boolean =
                newItem.date == oldItem.date

            override fun areContentsTheSame(oldItem: Record, newItem: Record): Boolean =
                newItem.date == oldItem.date
        }
    ) {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ConsoleLineViewHolder =
            ConsoleLineViewHolder(
                LayoutInflater.from(parent.context).inflate(
                    R.layout.fragment_ble_console_record, parent, false
                )
            )

        override fun getItemCount(): Int = consoleBuffer.size

        override fun onBindViewHolder(holder: ConsoleLineViewHolder, position: Int) {
            val item = getItem(position)
            holder.dateTimeTextView.text = item.dataText
            holder.valueTextView.text = item.text
            holder.view.tag = item
        }
    }
}
