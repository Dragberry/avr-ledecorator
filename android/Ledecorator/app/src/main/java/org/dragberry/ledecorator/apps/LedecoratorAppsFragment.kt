package org.dragberry.ledecorator.apps

import android.content.Context
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.synthetic.main.fragment_ledecorator_apps_item.view.*
import org.dragberry.ledecorator.BluetoothServiceHolder
import org.dragberry.ledecorator.MainActivity
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.bluetooth.DataFrames

private const val TAG = "LedecoratorAppsFragment"

private const val FRAME_RECEIVED = 3000

class LedecoratorAppFragment(private val onAppSelectedListener: (LedecoratorApp.() -> Unit)) :
    Fragment(), Handler.Callback {

    enum class Action(val value: Byte) {
        IDLE('I'.toByte()),
        LOAD('L'.toByte()),
        SAVE('S'.toByte()),
    }

    @Volatile
    private var action = Action.IDLE

    private lateinit var saveButton: Button

    private lateinit var loadButton: Button

    private var bluetoothService: MainActivity.BluetoothService? = null

    private val ledecoratorAppRecyclerViewAdapter: LedecoratorAppsRecyclerViewAdapter =
        LedecoratorAppsRecyclerViewAdapter(LedecoratorApps.APPS)

    private var selectedApp: Commands.App = Commands.App.IDLE

    private var handler: Handler? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        handler = Handler(this)
        if (context is BluetoothServiceHolder) {
            bluetoothService = context.bluetoothService
            bluetoothService?.onDataFrame(TAG) {
                if (DataFrames.check(this)) {
                    var updateRequired = false
                    LedecoratorApps.APPS.forEach {
                        val active = it.command == Commands.App.valueOf(get(1))
                        if (it.active != active) {
                            updateRequired = true
                        }
                        it.active = active
                    }
                    if (updateRequired) {
                        handler?.apply {
                            obtainMessage(FRAME_RECEIVED).sendToTarget()
                        }
                    }
                }
                Log.i(TAG, "$selectedApp")
                bluetoothService?.responseDataFrame = when (action) {
                    Action.LOAD -> DataFrames.loadAppsFrame
                    Action.SAVE -> DataFrames.saveAppsFrame(
                        Commands.App.CLOCK,
                        Commands.App.SNAKE,
                        Commands.App.WEATHER,
                        Commands.App.SANDBOX,
                        Commands.App.LIFE

                    )
                    else -> selectedApp.frame
                }
                action = Action.IDLE
            }
        } else {
            throw RuntimeException("$context must implement ${BluetoothServiceHolder::javaClass.name}")
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
            view.findViewById<RecyclerView>(R.id.ledecoratorAppsAppsListView)?.apply {
                layoutManager = LinearLayoutManager(context)
                adapter = ledecoratorAppRecyclerViewAdapter
            }
            saveButton = view.findViewById<Button>(R.id.ledecoratorAppsSaveButton).apply {
                setOnClickListener {
                    action = Action.SAVE
                }
            }
            loadButton = view.findViewById<Button>(R.id.ledecoratorAppsLoadButton).apply {
                setOnClickListener {
                    action = Action.LOAD
                }

            }
        }
        return view
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            FRAME_RECEIVED -> {
                ledecoratorAppRecyclerViewAdapter.notifyDataSetChanged()
            }
        }
        return true
    }

    inner class LedecoratorAppsRecyclerViewAdapter(private val appList: List<LedecoratorApp>) :
        ListAdapter<LedecoratorApp, LedecoratorAppsRecyclerViewAdapter.ViewHolder>(DiffCallback()) {

        private var selectedIndex: Int = -1

        init {
            submitList(appList)
        }

        inner class ViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
            val appNameTextView: TextView = view.ledecoratorAppNameTextView
            val appCodeTextView: TextView = view.ledecoratorAppCodeTextView
            val appStatusTextView: TextView = view.ledecoratorAppStatusTextView
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder =
            ViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.fragment_ledecorator_apps_item, parent, false))

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val item = appList[position]
            holder.apply {
                appNameTextView.text = item.name
                appCodeTextView.text = item.command.code.toString()
                appStatusTextView.apply {
                    if (item.active) {
                        text = getString(R.string.ledecorator_app_status_active)
                        visibility = View.VISIBLE
                        holder.view.setBackgroundColor(
                            if (selectedApp == Commands.App.IDLE)
                                Color.WHITE
                            else
                                Color.GREEN
                        )
                    } else {
                        text = getString(R.string.ledecorator_app_status_active)
                        visibility = View.GONE
                        holder.view.setBackgroundColor(Color.WHITE)
                    }
                }

                view.setOnLongClickListener {
                    if (selectedIndex == holder.adapterPosition) {
                        selectedIndex = -1
                        selectedApp = Commands.App.IDLE
                        it.setBackgroundColor(Color.WHITE)
                    }
                    true
                }

                view.setOnClickListener {
                    if (selectedIndex != holder.adapterPosition) {
                        selectedIndex = holder.adapterPosition
                        selectedApp = item.command
                    } else {
                        onAppSelectedListener.invoke(item)
                    }
                }
            }
        }
    }

    private class DiffCallback : DiffUtil.ItemCallback<LedecoratorApp>() {

        override fun areItemsTheSame(oldItem: LedecoratorApp, newItem: LedecoratorApp): Boolean {
            return oldItem.command == newItem.command
        }

        override fun areContentsTheSame(oldItem: LedecoratorApp, newItem: LedecoratorApp): Boolean {
            return oldItem.command == newItem.command
        }
    }
}
