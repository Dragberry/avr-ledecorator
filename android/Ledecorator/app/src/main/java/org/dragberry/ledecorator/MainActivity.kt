package org.dragberry.ledecorator

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun scanDevices(view: View)
    {
        val intent = Intent(this, ScanDevicesActivity::class.java)
        startActivity(intent)

    }
}
