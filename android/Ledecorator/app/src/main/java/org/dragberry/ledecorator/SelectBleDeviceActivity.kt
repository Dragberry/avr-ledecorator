package org.dragberry.ledecorator

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Intent
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView

private const val TAG = "SelectBleDeviceActivity"
private const val ENABLE_BT_REQUEST = 2
private const val ENABLE_FINE_LOCATION_REQUEST = 3

class SelectBleDeviceActivity : AppCompatActivity() {

    private lateinit var scanBleDevicesButton: Button

    private lateinit var bleDeviceListRecyclerView: RecyclerView
    private lateinit var bleDeviceListViewManager: RecyclerView.LayoutManager
    private lateinit var bleDeviceListAdapter: BleDeviceListAdapter
    private val bleDeviceList: MutableList<BluetoothDevice> = mutableListOf()

    private var scanning = false
    private val handler = Handler()

    private val bluetoothManager: BluetoothManager? by lazy {
        getSystemService(BluetoothManager::class.java)?.apply {
            startActivityForResult(Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), ENABLE_BT_REQUEST)
        }
        getSystemService(BluetoothManager::class.java)?.apply {
            startActivityForResult(Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), ENABLE_FINE_LOCATION_REQUEST)
        }
    }

    private val bluetoothAdapter: BluetoothAdapter? by lazy(LazyThreadSafetyMode.NONE) {
        bluetoothManager?.adapter
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_select_ble_device)

        scanBleDevicesButton = findViewById(R.id.scanBleDevicesButton)

        bleDeviceListViewManager = LinearLayoutManager(this)
        bleDeviceListAdapter = BleDeviceListAdapter().apply {
            submitList(bleDeviceList)
        }
        bleDeviceListRecyclerView = findViewById<RecyclerView>(R.id.availableBleDevicesListView).apply {
            layoutManager = bleDeviceListViewManager
            adapter = bleDeviceListAdapter

        }
    }

    private val scanCallback: ScanCallback  = object : ScanCallback()
    {
        override fun onScanResult(callbackType: Int, result: ScanResult?) {
            Log.i(TAG, "On scan result: $callbackType : $result")
            result?.apply {
                if (bleDeviceList.find { it.address == result.device.address } == null) {
                    bleDeviceList.add(result.device)
                }
                bleDeviceListAdapter.notifyDataSetChanged()
            }
        }
    }

    fun scanBleDevices(view: View) {
        if (scanning) {
            stopScan()
        } else {
            handler.postDelayed({ stopScan() }, 10000)
            startScan()
        }
    }

    private fun startScan() {
        Log.i(TAG, "Start scanning...")
        scanning = true
        scanBleDevicesButton.text = getString(R.string.stop)
        bleDeviceList.clear()
        bluetoothAdapter?.bluetoothLeScanner?.startScan(scanCallback)
    }

    private fun stopScan() {
        Log.i(TAG, "Stop scanning...")
        bluetoothAdapter?.bluetoothLeScanner?.stopScan(scanCallback)
        scanning = false
        scanBleDevicesButton.text = getString(R.string.scan)
        handler.removeCallbacksAndMessages(null)
    }

    private inner class BleDeviceListAdapter(private var selectedIndex: Int = -1) :
        ListAdapter<BluetoothDevice, BleDeviceListAdapter.BleDeviceViewHolder>(BleDeviceDiffCallback()) {

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
                                setResult(Activity.RESULT_OK, Intent().apply {
                                    putExtra(BluetoothDevice::class.java.name, item)
                                })
                                stopScan()
                                finish()
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
            BleDeviceViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.ble_device_view, parent, false))

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

