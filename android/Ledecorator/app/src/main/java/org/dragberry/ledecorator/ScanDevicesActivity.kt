package org.dragberry.ledecorator

import android.graphics.Color
import android.os.Bundle
import android.os.Parcelable
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.parcel.Parcelize

class ScanDevicesActivity : AppCompatActivity() {

    private lateinit var recyclerView: RecyclerView
    private lateinit var viewManager: RecyclerView.LayoutManager
    private lateinit var viewAdapter: BleDeviceAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_scan_devices)

        viewManager = LinearLayoutManager(this)
        viewAdapter = BleDeviceAdapter().apply {
            submitList(mutableListOf(
                BleDevice("111", "One"),
                BleDevice("222", "Two"),
                BleDevice("333", "Three"),
                BleDevice("444", "Four"),
                BleDevice("555", "Five"),
                BleDevice("666", "Six"),
                BleDevice("777", "Seven"),
                BleDevice("888", "Eight"),
                BleDevice("999", "Nine"),
                BleDevice("1010", "Ten"),
                BleDevice("1111", "Elven"),
                BleDevice("1212", "Twelve")
            ))
        }
        recyclerView = findViewById<RecyclerView>(R.id.availableDevicesListView).apply {
            layoutManager = viewManager
            adapter = viewAdapter

        }
    }

    private class BleDeviceAdapter(private var selectedIndex: Int = -1) :
        ListAdapter<BleDevice, BleDeviceAdapter.BleDeviceViewHolder>(BleDeviceDiffCallback()) {

        inner class BleDeviceViewHolder(private val view: View) : RecyclerView.ViewHolder(view) {

            fun bind(item: BleDevice) {
                view.apply {
                    setBackgroundColor(if (selectedIndex == adapterPosition) Color.GRAY else Color.WHITE)

                    findViewById<TextView>(R.id.deviceNameTextView).text = item.name
                    findViewById<TextView>(R.id.deviceAddressTextView).text = item.address
                    setOnClickListener {
                        if (selectedIndex != adapterPosition) {
                            notifyItemChanged(selectedIndex)
                            selectedIndex = adapterPosition
                        }
                        Log.i(BleDeviceAdapter::class.java.name, "Clicked: ${item.name}")
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

    private class BleDeviceDiffCallback : DiffUtil.ItemCallback<BleDevice>() {

        override fun areItemsTheSame(oldItem: BleDevice, newItem: BleDevice): Boolean {
            return oldItem.address == newItem.address
        }

        override fun areContentsTheSame(oldItem: BleDevice, newItem: BleDevice): Boolean {
            return oldItem.address == newItem.address
        }
    }

    @Parcelize
    class BleDevice(val address: String, val name: String) : Parcelable

}

