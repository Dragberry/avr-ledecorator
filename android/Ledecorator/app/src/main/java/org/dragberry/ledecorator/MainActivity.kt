package org.dragberry.ledecorator

import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import org.dragberry.ledecorator.apps.LedecoratorAppFragment
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame
import org.dragberry.ledecorator.bluetooth.DataFrameHandler
import org.dragberry.ledecorator.bluetooth.fragment.BleDeviceSelectionFragment
import java.nio.charset.StandardCharsets
import java.util.*

private const val TAG = "MainActivity"

private const val ACTIVE_LEDECORATOR_APP = "ActiveLedecoratorApp"

private const val ENABLE_BT_REQUEST = 100

private const val ENABLE_FINE_LOCATION_REQUEST = 101

class MainActivity :
    AppCompatActivity(),
    BluetoothServiceHolder {

    override val bluetoothService: BluetoothService by lazy {
        BluetoothService()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    override fun onDestroy() {
        super.onDestroy()
        bluetoothService.disconnect()
    }

    fun startBleDeviceSelection() {
        supportFragmentManager.findFragmentById(R.id.mainFragmentLayout).apply {
            supportFragmentManager
                .beginTransaction()
                .replace(
                    R.id.mainFragmentLayout,
                    BleDeviceSelectionFragment(),
                    ACTIVE_LEDECORATOR_APP
                )
                .commit()
        }
    }

    fun showApps() {
        supportFragmentManager.findFragmentById(R.id.mainFragmentLayout).apply {
            supportFragmentManager
                .beginTransaction()
                .replace(
                    R.id.mainFragmentLayout,
                    LedecoratorAppFragment.newInstance(),
                    ACTIVE_LEDECORATOR_APP
                )
                .commit()
        }
    }

    fun hideApps() {
        supportFragmentManager.findFragmentByTag(ACTIVE_LEDECORATOR_APP)?.apply {
            supportFragmentManager
                .beginTransaction()
                .remove(this)
                .commit()
        }
    }

    inner class BluetoothService {

        private val serviceUUID = UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb")

        private val characteristicUUID = UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb")

        private val tag = "BluetoothService"

        private var deviceSelectedCallback: (BluetoothDevice?.() -> Unit)? = null

        private var scanning = false

        private val scanningHandler: Handler = Handler()

        var deviceScanCallback: Handler? = null

        private var bluetoothDevice: BluetoothDevice? = null

        private var bluetoothGatt: BluetoothGatt? = null

        private val bluetoothManager: BluetoothManager? by lazy {
            getSystemService(android.bluetooth.BluetoothManager::class.java)?.apply {
                startActivityForResult(
                    Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    ENABLE_BT_REQUEST
                )
                startActivityForResult(
                    Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    ENABLE_FINE_LOCATION_REQUEST
                )
            }
        }

        private val defaultDataFrameHandler: (ByteArray.() -> ByteArray) = { BleInterchangeFrame.IDLE }

        private var dataFrameHandler: (ByteArray.() -> ByteArray) = defaultDataFrameHandler

        fun startDeviceSelection(deviceSelectedCallback: BluetoothDevice?.() -> Unit) {
            this.deviceSelectedCallback = deviceSelectedCallback
            startBleDeviceSelection()
        }

        fun completeDeviceSelection(selectedBluetoothDevice: BluetoothDevice?) {
            bluetoothDevice = selectedBluetoothDevice
            stopScan()
            deviceSelectedCallback?.invoke(bluetoothDevice)
        }

        fun toggleScan() {
            if (scanning) {
                stopScan()
            } else {
                scanningHandler.postDelayed({ stopScan() }, 10000)
                startScan()
            }
        }

        private val scanCallback: ScanCallback = object : ScanCallback()
        {
            override fun onScanResult(callbackType: Int, result: ScanResult?) {
                Log.i(tag, "On scan result: $callbackType : $result")
                result?.apply {
                    deviceScanCallback?.apply {
                        obtainMessage(LEDECORATOR_ON_SCAN_RESULT, result.device).sendToTarget()
                    }
                }
            }
        }

        private fun startScan() {
            Log.i(TAG, "Start scanning...")
            bluetoothManager?.apply {
                scanning = true
                adapter.bluetoothLeScanner.startScan(scanCallback)
                deviceScanCallback?.apply {
                    obtainMessage(LEDECORATOR_SCAN_STARTED).sendToTarget()
                }
            }
        }

        private fun stopScan() {
            Log.i(TAG, "Stop scanning")
            bluetoothManager?.apply {
                scanningHandler.removeCallbacksAndMessages(null)
                scanning = false
                adapter.bluetoothLeScanner.stopScan(scanCallback)
                deviceScanCallback?.apply {
                    obtainMessage(LEDECORATOR_SCAN_STOPPED).sendToTarget()
                }
            }
        }

        var connectionHandler: Handler? = null

        private val gattCallback = object : BluetoothGattCallback() {

            override fun onConnectionStateChange(
                gatt: BluetoothGatt?,
                status: Int,
                newState: Int
            ) {
                Log.i(tag, "onConnectionStateChange: $gatt | status: $status | newState: $newState")
                if (status == 0) {
                    when (newState) {
                        BluetoothProfile.STATE_CONNECTED -> {
                            bluetoothGatt?.discoverServices()
                        }
                        BluetoothProfile.STATE_DISCONNECTED -> {
                            hideApps()
                            connectionHandler?.apply {
                                obtainMessage(LEDECORATOR_STATE_DISCONNECTED).sendToTarget()
                            }
                        }
                    }
                } else {
                    connectionHandler?.apply {
                        obtainMessage(LEDECORATOR_STATE_CONNECTION_ERROR, status).sendToTarget()
                    }
                }
            }

            override fun onServicesDiscovered(gatt: BluetoothGatt?, status: Int) {
                Log.i(tag, "onServicesDiscovered ${gatt?.services} - $status")
                gatt?.apply {
                    getService(serviceUUID)?.apply {
                        getCharacteristic(characteristicUUID)?.apply {
                            setCharacteristicNotification(this, true)
                        }
                    }
                }
                connectionHandler?.apply {
                    obtainMessage(LEDECORATOR_STATE_CONNECTED, gatt).sendToTarget()
                }
                showApps()
            }

            override fun onCharacteristicChanged(
                gatt: BluetoothGatt?,
                characteristic: BluetoothGattCharacteristic?
            ) {
                Log.i(tag, "onCharacteristicChanged")

                gatt?.writeCharacteristic(characteristic?.apply {
                    value = dataFrameHandler(value)
                })
            }
        }

        fun connect() {
            bluetoothGatt = bluetoothDevice?.connectGatt(this@MainActivity, false, gattCallback)
        }

        fun disconnect() {
            stopScan()
            bluetoothGatt?.disconnect()
            bluetoothGatt = null
        }

        fun onDataFrame(handler: (ByteArray.() -> ByteArray)?) {
            dataFrameHandler = handler ?: defaultDataFrameHandler
        }

    }
}

interface BluetoothServiceHolder {
    val bluetoothService: MainActivity.BluetoothService
}
