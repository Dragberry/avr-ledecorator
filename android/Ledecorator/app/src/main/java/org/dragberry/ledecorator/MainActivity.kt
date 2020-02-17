package org.dragberry.ledecorator

import android.app.Activity
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

private const val SELECT_BLE_DEVICE_REQUEST = 1

class MainActivity : AppCompatActivity() {

    private lateinit var selectedBleDeviceStatusTextView: TextView
    private lateinit var selectedBleDeviceTextView: TextView
    private lateinit var connectBleDeviceButton: Switch

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        selectedBleDeviceStatusTextView = findViewById(R.id.selectedBleDeviceStatusTextView)
        selectedBleDeviceTextView = findViewById(R.id.selectedBleDeviceTextView)
        connectBleDeviceButton = findViewById<Switch>(R.id.connectBleDeviceButton).apply {
            visibility = View.INVISIBLE
            setOnCheckedChangeListener { _, isChecked ->
                selectedBleDeviceStatusTextView.text = getString(
                    if (isChecked) R.string.status_connected else R.string.status_disconnected
                )
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
                val device = data?.getParcelableExtra<BluetoothDevice>(BluetoothDevice::class.java.name)
                if (device != null) {
                    connectBleDeviceButton.visibility = View.VISIBLE
                    selectedBleDeviceTextView.text = device.name ?: device.address
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
