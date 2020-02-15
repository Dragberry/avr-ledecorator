package org.dragberry.ledecorator

import android.app.Activity
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View

private const val SELECT_BLE_DEVICE_REQUEST = 1

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == SELECT_BLE_DEVICE_REQUEST)
        {
            if (resultCode == Activity.RESULT_OK)
            {
                Log.i(MainActivity::class.java.name, "Result is OK")
                if (data != null && data.extras != null)
                {
                    Log.i(MainActivity::class.java.name, "Extra is here")
                }
            }
        }
    }

    fun scanDevices(view: View)
    {
        val intent = Intent(this, SelectBleDeviceActivity::class.java)

        startActivityForResult(intent, SELECT_BLE_DEVICE_REQUEST)

    }
}
