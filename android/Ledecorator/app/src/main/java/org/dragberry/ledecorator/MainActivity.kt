package org.dragberry.ledecorator

import android.app.Activity
import android.bluetooth.*
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import java.nio.charset.StandardCharsets
import java.util.*

private const val GENERIC_CHARACTERISTIC = "00001801-0000-1000-8000-00805f9b34fb"

private const val TAG = "MainActivity"

private const val SELECT_BLE_DEVICE_REQUEST = 1

class MainActivity : AppCompatActivity() {

    private val GENERIC_CHARACTERISTIC_UUID = UUID.fromString(GENERIC_CHARACTERISTIC)

    private lateinit var selectedBleDeviceStatusTextView: TextView
    private lateinit var selectedBleDeviceTextView: TextView
    private lateinit var connectBleDeviceButton: Switch

    private var bluetoothGatt: BluetoothGatt? = null
    private var bluetoothDevice: BluetoothDevice? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        selectedBleDeviceStatusTextView = findViewById(R.id.selectedBleDeviceStatusTextView)
        selectedBleDeviceTextView = findViewById(R.id.selectedBleDeviceTextView)
        connectBleDeviceButton = findViewById<Switch>(R.id.connectBleDeviceButton).apply {
            visibility = View.INVISIBLE
            setOnCheckedChangeListener { _, isChecked ->
                if (isChecked) {
                    bluetoothGatt = bluetoothDevice?.connectGatt(this@MainActivity, true, GattCallback())
                } else {
                    if (bluetoothGatt != null) {
                        bluetoothGatt?.disconnect()
                    }
                }
            }
        }
    }

    inner class GattCallback : BluetoothGattCallback() {

        override fun onConnectionStateChange(gatt: BluetoothGatt?, status: Int, newState: Int) {
            Log.i(TAG, "onConnectionStateChange: $gatt | status: $status | newState: $newState")
            when (newState) {
                BluetoothProfile.STATE_CONNECTED -> {
                    selectedBleDeviceStatusTextView.text = getString(R.string.status_connected)
                    bluetoothGatt?.discoverServices()
                }
                else -> {
                    selectedBleDeviceStatusTextView.text = getString(R.string.status_disconnected)
                }
            }
        }

        override fun onCharacteristicChanged(
            gatt: BluetoothGatt?,
            characteristic: BluetoothGattCharacteristic?
        ) {
            Log.i(TAG, "onCharacteristicChanged: $gatt | characteristic: ${characteristic?.value?.toString(StandardCharsets.US_ASCII)}")
            gatt?.writeCharacteristic(characteristic?.apply { value = "aaaaaaaaaaaaaaaaaaaa".toByteArray(StandardCharsets.US_ASCII) } )
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt?, status: Int) {
            Log.i(TAG, "onServicesDiscovered: ${gatt} | status: $status")

            gatt?.getService(UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb"))?.apply {
                getCharacteristic(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"))?.apply {
                    gatt.setCharacteristicNotification(this, true)
                }
            }
        }

    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == SELECT_BLE_DEVICE_REQUEST)
        {
            if (resultCode == Activity.RESULT_OK)
            {
                Log.i(MainActivity::class.java.name, "Result is OK")
                bluetoothDevice = data?.getParcelableExtra(BluetoothDevice::class.java.name)

                if (bluetoothDevice != null) {
                    connectBleDeviceButton.visibility = View.VISIBLE
                    selectedBleDeviceTextView.text = bluetoothDevice?.name ?: bluetoothDevice?.address
                } else {
                    connectBleDeviceButton.visibility = View.GONE
                    selectedBleDeviceTextView.text = null
                }
            }
        }
    }

    fun selectBleDevice(view: View)
    {
        val intent = Intent(this, SelectBleDeviceActivity::class.java)

        startActivityForResult(intent, SELECT_BLE_DEVICE_REQUEST)

    }

    fun connectBleDevice() {

    }
}
