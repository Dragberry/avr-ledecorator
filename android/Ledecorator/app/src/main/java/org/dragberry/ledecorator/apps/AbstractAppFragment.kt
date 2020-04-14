package org.dragberry.ledecorator.apps

import android.content.Context
import android.os.Handler
import android.os.Message
import androidx.fragment.app.Fragment
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.MainActivity

abstract class AbstractAppFragment : Fragment(), Handler.Callback {

    private var bluetoothService: MainActivity.BluetoothService? = null

    protected abstract val fragmentId: String

    protected var handler: Handler? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is BluetoothServiceHolder) {
            handler = Handler(this)
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(fragmentId) {
                bluetoothService?.responseDataFrame = onDataFrame(this)
            }
        }
    }

    protected abstract fun onDataFrame(bytes: ByteArray): ByteArray

    override fun onDetach() {
        super.onDetach()
        handler?.removeCallbacksAndMessages(null)
        handler = null
        bluetoothService?.onDataFrame(fragmentId, null)
        bluetoothService = null
    }

    override fun handleMessage(msg: Message): Boolean {
        return true
    }
}