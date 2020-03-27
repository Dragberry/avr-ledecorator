package org.dragberry.ledecorator.bluetooth

import android.app.Activity
import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Intent
import android.os.Handler
import android.util.Log
import java.util.*
import kotlin.properties.Delegates

private const val TAG = "BleService"

private const val ENABLE_BT_REQUEST = 100

private const val ENABLE_FINE_LOCATION_REQUEST = 1001

const val LEDECORATOR_STATE_CONNECTING = 1001

const val LEDECORATOR_STATE_CONNECTED = 1002

const val LEDECORATOR_STATE_DISCONNECTING = 1003

const val LEDECORATOR_STATE_DISCONNECTED = 1004

const val LEDECORATOR_STATE_CONNECTION_ERROR = 1005

const val LEDECORATOR_DATAFRAME_RECIEVED = 2000

const val LEDECORATOR_SCAN_STARTED = 3000

const val LEDECORATOR_SCAN_STOPPED = 3001

const val LEDECORATOR_ON_SCAN_RESULT = 3002

class BleService(private val activity: Activity, val mainHandler: Handler) {

    private val serviceUUID = UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb")

    private val characteristicUUID = UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb")

    val bleDeviceList: MutableList<BluetoothDevice> = mutableListOf()

    private val scanningHandler: Handler = Handler()

    private var scanning = false

    private val bluetoothManager: BluetoothManager? by lazy {
        with (activity) {
            getSystemService(BluetoothManager::class.java)?.apply {
                startActivityForResult(Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), ENABLE_BT_REQUEST)
                startActivityForResult(Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), ENABLE_FINE_LOCATION_REQUEST)
            }
        }
    }

    var bluetoothDevice by Delegates.observable<BluetoothDevice?>(null) { _, _, _ ->
        if (scanning) stopScan()
    }

    private var bluetoothGatt: BluetoothGatt? = null

    private var bluetoothGattCharacteristic: BluetoothGattCharacteristic? = null

    private val scanCallback: ScanCallback = object : ScanCallback()
    {
        override fun onScanResult(callbackType: Int, result: ScanResult?) {
            Log.i(TAG, "On scan result: $callbackType : $result")
            result?.apply {
                if (bleDeviceList.find { it.address == result.device.address } == null) {
                    bleDeviceList.add(result.device)
                }
                mainHandler.obtainMessage(LEDECORATOR_ON_SCAN_RESULT).sendToTarget()
            }
        }
    }

    fun toggleScan() {
        if (scanning) {
            stopScan()
        } else {
            scanningHandler.postDelayed({ stopScan() }, 10000)
            startScan()
        }
    }

    private fun startScan() {
        Log.i(TAG, "Start scanning...")
        bluetoothManager?.apply {
            scanning = true
            bleDeviceList.clear()
            adapter.bluetoothLeScanner.startScan(scanCallback)
            mainHandler.obtainMessage(LEDECORATOR_SCAN_STARTED).sendToTarget()
        }
    }

    private fun stopScan() {
        Log.i(TAG, "Stop scanning")
        bluetoothManager?.apply {
            scanningHandler.removeCallbacksAndMessages(null)
            scanning = false
            adapter.bluetoothLeScanner.stopScan(scanCallback)
            mainHandler.obtainMessage(LEDECORATOR_SCAN_STOPPED).sendToTarget()
        }
    }

    fun connect() {
        Log.i(TAG, "Connecting...")
        mainHandler.obtainMessage(LEDECORATOR_STATE_CONNECTING).sendToTarget()
        bluetoothGatt =
            bluetoothDevice?.connectGatt(activity, false, BleGattCallback())
    }

    fun disconnect() {
        Log.i(TAG, "Disonnecting...")
        bluetoothGattCharacteristic = null
        mainHandler.obtainMessage(LEDECORATOR_STATE_DISCONNECTING).sendToTarget()
        bluetoothGatt?.apply {
            disconnect()
        }
    }

    fun dispose() {
        bluetoothGattCharacteristic = null
        bluetoothGatt?.close()
        bluetoothGatt?.disconnect()
        bluetoothGatt = null
    }

    fun send(dataFrame: ByteArray) {
        bluetoothGattCharacteristic?.apply {
            value = dataFrame
            bluetoothGatt?.writeCharacteristic(this)
        }
    }

    inner class BleGattCallback : BluetoothGattCallback() {

        override fun onConnectionStateChange(gatt: BluetoothGatt?, status: Int, newState: Int) {
            Log.i(TAG, "onConnectionStateChange: $gatt | status: $status | newState: $newState")
            if (status == 0) {
                when (newState) {
                    BluetoothProfile.STATE_CONNECTED -> {
                        mainHandler.obtainMessage(LEDECORATOR_STATE_CONNECTED).sendToTarget()
                        bluetoothGatt?.discoverServices()
                    }
                    BluetoothProfile.STATE_DISCONNECTED -> {
                        mainHandler.obtainMessage(LEDECORATOR_STATE_DISCONNECTED).sendToTarget()
                    }
                }
            } else {
                mainHandler.obtainMessage(LEDECORATOR_STATE_CONNECTION_ERROR, status).sendToTarget()
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt?, status: Int) {
            bluetoothGatt?.apply {
                getService(serviceUUID)?.apply {
                    getCharacteristic(characteristicUUID)?.apply {
                        bluetoothGattCharacteristic = this
                        setCharacteristicNotification(this, true)
                    }
                }
            }
            mainHandler.obtainMessage(LEDECORATOR_STATE_CONNECTED, gatt).sendToTarget()
        }

        override fun onCharacteristicChanged(
            gatt: BluetoothGatt?,
            characteristic: BluetoothGattCharacteristic?
        ) {
            mainHandler.obtainMessage(LEDECORATOR_DATAFRAME_RECIEVED, characteristic?.value).sendToTarget()
        }
    }

}