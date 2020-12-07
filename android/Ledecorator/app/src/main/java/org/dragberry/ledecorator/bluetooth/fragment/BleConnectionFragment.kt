package org.dragberry.ledecorator.bluetooth.fragment

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Switch
import android.widget.TextView
import kotlinx.android.synthetic.main.fragment_ble_device_selection_item.*
import org.dragberry.ledecorator.*

private const val TAG = "BleConnectionFragment"

class BleConnectionFragment : Fragment(), Handler.Callback {

    private lateinit var selectBleDeviceButton: Button
    private lateinit var selectedBleDeviceStatusTextView: TextView
    private lateinit var selectedBleDeviceTextView: TextView
    private lateinit var connectBleDeviceSwitch: Switch

    private var bluetoothService: MainActivity.BluetoothService? = null

    private var handler: Handler? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        Log.i(TAG, "onCreateView")
        return inflater.inflate(R.layout.fragment_ble_connection, container, false).apply {
            selectBleDeviceButton = findViewById<Button>(R.id.selectBleDeviceButton).apply {
                setOnClickListener { onBleDeviceSelection() }
            }
            selectedBleDeviceStatusTextView = findViewById<TextView>(R.id.selectedBleDeviceStatusTextView).apply {
                text = getString(R.string.status_no_device)
            }
            selectedBleDeviceTextView = findViewById(R.id.selectedBleDeviceTextView)
            connectBleDeviceSwitch = findViewById<Switch>(R.id.connectBleDeviceButton).apply {
                isEnabled = false
                setOnCheckedChangeListener { _, isChecked ->
                    if (isChecked) {
                        onConnecting()
                    } else {
                        if (bluetoothService!!.connected) {
                            onDisconnecting()
                        }
                    }
                }
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        Log.i(TAG, "onAttach")
        if (context is BluetoothServiceHolder) {
            handler = Handler(this)
            bluetoothService = context.bluetoothService.apply {
                connectionHandler = handler
            }
        } else {
            throw RuntimeException("$context must implement BluetoothServiceHolder")
        }
    }

    override fun onDetach() {
        super.onDetach()
        Log.i(TAG, "onDetach")
        handler?.removeCallbacksAndMessages(null)
        handler = null
        bluetoothService?.connectionHandler = null
        bluetoothService = null
    }

    private fun onBleDeviceSelection() {
        connectBleDeviceSwitch.isEnabled = false
        bluetoothService?.startDeviceSelection {
            if (this != null) {
                selectedBleDeviceTextView.text = name ?: address
                selectedBleDeviceStatusTextView.text = getString(R.string.status_disconnected)
                connectBleDeviceSwitch.isEnabled = true
            } else {
                deviceNameTextView.text = null
                selectedBleDeviceStatusTextView.text = getString(R.string.status_no_device)
                connectBleDeviceSwitch.isEnabled = false
            }
        }
    }

    private fun onConnecting() {
        selectBleDeviceButton.isEnabled = false
        connectBleDeviceSwitch.isEnabled = false
        selectedBleDeviceStatusTextView.text = getString(R.string.status_connecting)
        bluetoothService?.connect()
    }

    private fun onConnected() {
        selectBleDeviceButton.isEnabled = false
        connectBleDeviceSwitch.isEnabled = true
        selectedBleDeviceStatusTextView.text = getString(R.string.status_connected)
    }

    private fun onDisconnecting() {
        selectBleDeviceButton.isEnabled = false
        connectBleDeviceSwitch.isEnabled = false
        selectedBleDeviceStatusTextView.text = getString(R.string.status_disconnecting)
        bluetoothService?.disconnect()
    }

    private fun onDisconnected() {
        selectBleDeviceButton.isEnabled = true
        connectBleDeviceSwitch.isEnabled = true
        connectBleDeviceSwitch.isChecked = false
        selectedBleDeviceStatusTextView.text = getString(R.string.status_disconnected)
    }

    private fun onConnectionError(error: String) {
        selectBleDeviceButton.isEnabled = true
        connectBleDeviceSwitch.isEnabled = true
        connectBleDeviceSwitch.isChecked = false
        selectedBleDeviceStatusTextView.text = getString(R.string.status_connection_error, error)
    }

    override fun handleMessage(msg: Message): Boolean {
        Log.i(TAG, "handleMessage: ${msg.what}")
        when (msg.what) {
            LEDECORATOR_STATE_CONNECTED -> {
                onConnected()
            }
            LEDECORATOR_STATE_DISCONNECTED -> {
                onDisconnected()
            }
            LEDECORATOR_STATE_CONNECTION_ERROR -> {
                onConnectionError(msg.obj.toString())
            }
        }
        return true
    }
}
