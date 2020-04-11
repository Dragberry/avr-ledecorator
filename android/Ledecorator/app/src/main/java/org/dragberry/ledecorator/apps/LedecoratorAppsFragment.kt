package org.dragberry.ledecorator.apps

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.MainActivity
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.bluetooth.BleInterchangeFrame
import java.lang.RuntimeException
import java.nio.charset.StandardCharsets

private const val TAG = "LedecoratorAppFragment"

class LedecoratorAppFragment : Fragment() {

    private var bluetoothService: MainActivity.BluetoothService? = null

    private val ledecoratorAppRecyclerViewAdapter: LedecoratorAppRecyclerViewAdapter =
        LedecoratorAppRecyclerViewAdapter(LedecoratorApps.APPS)


    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame {
                Log.i(TAG, toString(StandardCharsets.US_ASCII))
                BleInterchangeFrame.IDLE
            }
        } else {
            throw RuntimeException("$context must implement BluetoothServiceHolder")
        }
    }

    override fun onDetach() {
        super.onDetach()
        bluetoothService?.onDataFrame(null)
        bluetoothService = null
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_ledecorator_apps, container, false)
        if (view is RecyclerView) {
            with(view) {
                layoutManager = LinearLayoutManager(context)
                adapter = ledecoratorAppRecyclerViewAdapter
            }
        }
        return view
    }

    companion object {

        @JvmStatic
        fun newInstance() = LedecoratorAppFragment()
    }
}