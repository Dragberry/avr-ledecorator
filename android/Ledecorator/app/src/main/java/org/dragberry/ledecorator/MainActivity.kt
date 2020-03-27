package org.dragberry.ledecorator

import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import org.dragberry.ledecorator.apps.LedecoratorAppFragment
import org.dragberry.ledecorator.bluetooth.*
import org.dragberry.ledecorator.bluetooth.fragment.BleDeviceSelectionFragment
import java.util.*

private const val TAG = "MainActivity"

private const val ACTIVE_LEDECORATOR_APP = "ActiveLedecoratorApp"

class MainActivity :
    AppCompatActivity(),
    BluetoothServiceHolder {

    override val bluetoothService: BluetoothService by lazy {
        BluetoothService(this)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
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

    inner class BluetoothService(
        private val context: Context
    ) {

        private val serviceUUID = UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb")

        private val characteristicUUID = UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb")

        private val tag = "BluetoothService"

        private val enableBTRequest = 100

        private val enableFineLocationRequest = 1001

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
                    enableBTRequest
                )
                startActivityForResult(
                    Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    enableFineLocationRequest
                )
            }
        }

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
                // TODO: Gatt disconnect
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

        fun connect(handler: Handler) {
            bluetoothGatt = bluetoothDevice?.let {
                Log.i(tag, "Connection to ${it.name ?: it.address}")
                it.connectGatt(context, false, object : BluetoothGattCallback() {

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
                                    handler.obtainMessage(LEDECORATOR_STATE_DISCONNECTED).sendToTarget()
                                }
                            }
                        } else {
                            handler.obtainMessage(LEDECORATOR_STATE_CONNECTION_ERROR, status).sendToTarget()
                        }
                    }

                    override fun onServicesDiscovered(gatt: BluetoothGatt?, status: Int) {
                        bluetoothGatt?.apply {
                            getService(serviceUUID)?.apply {
                                getCharacteristic(characteristicUUID)?.apply {
                                    setCharacteristicNotification(this, true)
                                }
                            }
                        }
                        handler.obtainMessage(LEDECORATOR_STATE_CONNECTED, gatt).sendToTarget()
                        showApps()
                    }

                })
            }
        }

        fun disconnect(handler: Handler) {

        }
    }
}

interface BluetoothServiceHolder {
    val bluetoothService: MainActivity.BluetoothService
}
