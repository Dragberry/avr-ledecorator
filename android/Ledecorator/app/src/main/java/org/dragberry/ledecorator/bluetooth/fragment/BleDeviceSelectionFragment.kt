package org.dragberry.ledecorator.bluetooth.fragment

import android.bluetooth.BluetoothDevice
import android.content.Context
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import org.dragberry.ledecorator.*

private const val TAG = "BleDeviceSelectionFragment"

class BleDeviceSelectionFragment : Fragment(), Handler.Callback {

    private lateinit var scanBleDevicesButton: Button

    private var bluetoothService: MainActivity.BluetoothService? = null

    private var handler: Handler? = null

    private lateinit var bleDeviceListAdapter: BleDeviceListAdapter

    private val bleDeviceList = mutableListOf<BluetoothDevice>()

    override fun onAttach(context: Context) {
        super.onAttach(context)
        handler = Handler(this)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService.apply {
                deviceScanCallback = handler
            }
        } else {
            throw RuntimeException("$context must implement BluetoothServiceHolder")
        }
    }

    override fun onDetach() {
        super.onDetach()
        handler?.removeCallbacksAndMessages(null)
        handler = null
        bluetoothService?.deviceScanCallback = null
        bluetoothService = null
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_ble_device_selection, container, false)
        if (view != null) {
            bleDeviceListAdapter = BleDeviceListAdapter()
            bleDeviceListAdapter.submitList(bleDeviceList)

            scanBleDevicesButton = view.findViewById(R.id.scanBleDevicesButton)
            scanBleDevicesButton.setOnClickListener {
                bluetoothService?.toggleScan()
            }

            view.findViewById<RecyclerView>(R.id.availableBleDevicesListView)?.apply {
                layoutManager = LinearLayoutManager(view.context)
                adapter = bleDeviceListAdapter
            }
        }
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            LEDECORATOR_ON_SCAN_RESULT -> {
                val device: BluetoothDevice = msg.obj as BluetoothDevice
                if (bleDeviceList.find { it.address == device.address } == null) {
                    bleDeviceList.add(device)
                    bleDeviceListAdapter.notifyDataSetChanged()
                }
            }
            LEDECORATOR_SCAN_STARTED -> {
                bleDeviceList.clear()
                scanBleDevicesButton.text = getString(R.string.stop)
                bleDeviceListAdapter.resetSelection()
                bleDeviceListAdapter.notifyDataSetChanged()
            }
            LEDECORATOR_SCAN_STOPPED -> {
                scanBleDevicesButton.text = getString(R.string.scan)
            }
        }
        return true
    }

    private inner class BleDeviceListAdapter(private var selectedIndex: Int = -1) :
        ListAdapter<BluetoothDevice, BleDeviceListAdapter.BleDeviceViewHolder>(BleDeviceDiffCallback()) {

        fun resetSelection() {
            selectedIndex = -1
        }

        inner class BleDeviceViewHolder(private val view: View) : RecyclerView.ViewHolder(view) {

            private var nameView: TextView
            private var addressView: TextView
            private var actionButton: Button

            init {
                view.apply {
                    nameView = findViewById(R.id.deviceNameTextView)
                    addressView = findViewById(R.id.deviceAddressTextView)
                    actionButton = findViewById(R.id.selectBleDeviceButton)
                }
            }

            fun bind(item: BluetoothDevice) {
                nameView.text = item.name
                addressView.text = item.address
                actionButton.visibility = View.GONE

                view.apply {
                    setBackgroundColor(if (selectedIndex == adapterPosition) Color.GRAY else Color.WHITE)
                    setOnClickListener {
                        setBackgroundColor(Color.GRAY)
                        actionButton.apply {
                            visibility = View.VISIBLE
                            setOnClickListener {
                                bluetoothService?.completeDeviceSelection(item)
                            }
                        }
                        if (selectedIndex != adapterPosition) {
                            notifyItemChanged(selectedIndex)
                            selectedIndex = adapterPosition
                        }
                        Log.i(BleDeviceListAdapter::class.java.name, "Clicked: ${item.name}")
                    }
                }
            }
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): BleDeviceViewHolder =
            BleDeviceViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.fragment_ble_device_selection_item, parent, false))

        override fun onBindViewHolder(holder: BleDeviceViewHolder, position: Int) {
            holder.bind(getItem(position))
        }
    }

    private class BleDeviceDiffCallback : DiffUtil.ItemCallback<BluetoothDevice>() {

        override fun areItemsTheSame(oldItem: BluetoothDevice, newItem: BluetoothDevice): Boolean {
            return oldItem.address == newItem.address
        }

        override fun areContentsTheSame(oldItem: BluetoothDevice, newItem: BluetoothDevice): Boolean {
            return oldItem.address == newItem.address
        }
    }
}