package org.dragberry.ledecorator.apps

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Message
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

private const val TAG = "LedecoratorAppFragment"

class LedecoratorAppFragment : Fragment(), Handler.Callback {

    private var bluetoothService: MainActivity.BluetoothService? = null

    private val ledecoratorAppRecyclerViewAdapter: LedecoratorAppsRecyclerViewAdapter =
        LedecoratorAppsRecyclerViewAdapter(LedecoratorApps.APPS)

    private var handler: Handler? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        handler = Handler(this)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(TAG) {
                if (BleInterchangeFrame.FRAME_START == get(0) && BleInterchangeFrame.FRAME_END == get(19)) {
                    LedecoratorApps.APPS.forEach {
                        it.active = it.code == get(1)
                    }
                    handler?.apply {
                        obtainMessage(33).sendToTarget()
                    }
                }
                bluetoothService?.responseDataFrame = BleInterchangeFrame.IDLE
            }
        } else {
            throw RuntimeException("$context must implement BluetoothServiceHolder")
        }
    }

    override fun onDetach() {
        super.onDetach()
        bluetoothService?.onDataFrame(TAG, null)
        bluetoothService = null
        handler?.removeCallbacksAndMessages(null)
        handler = null
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_ledecorator_apps, container, false)
        if (view != null) {
            view.findViewById<RecyclerView>(R.id.fragment_ledecorator_apps_list)?.apply {
                layoutManager = LinearLayoutManager(context)
                adapter = ledecoratorAppRecyclerViewAdapter
            }
        }
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            33 -> {
                ledecoratorAppRecyclerViewAdapter.notifyDataSetChanged()
            }
        }
        return true
    }


    companion object {

        @JvmStatic
        fun newInstance() = LedecoratorAppFragment()
    }
}
