package org.dragberry.ledecorator

import android.app.Activity
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

private const val SELECT_BLE_DEVICE_REQUEST = 1

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == SELECT_BLE_DEVICE_REQUEST)
        {
            if (resultCode == Activity.RESULT_OK)
            {
                Log.i(MainActivity::class.java.name, "Result is OK")
                data?.getParcelableExtra<BluetoothDevice>(BluetoothDevice::class.java.name)?.apply {
                    findViewById<TextView>(R.id.selectedBleDeviceTextView).text = name
                    Log.i(MainActivity::class.java.name, "$name: $address")
                }
            }
        }
    }

    fun selectBleDevice(view: View)
    {
        val intent = Intent(this, SelectBleDeviceActivity::class.java)

        startActivityForResult(intent, SELECT_BLE_DEVICE_REQUEST)

    }
}
