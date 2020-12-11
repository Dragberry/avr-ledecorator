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
import android.widget.CheckBox
import android.widget.ImageButton
import android.widget.TextView
import androidx.core.content.ContextCompat
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

private const val STOP_EDITING = 3001

class LedecoratorAppFragment(private val onAppSelectedListener: (LedecoratorApp.() -> Unit)) :
    Fragment(), Handler.Callback {

    enum class Action(val value: Byte) {
        IDLE('I'.toByte()),
        LOAD('L'.toByte()),
        SAVE('S'.toByte()),
    }

    @Volatile
    private var action = Action.IDLE

    @Volatile
    private var editable = false

    private lateinit var saveButton: Button

    private lateinit var cancelButton: Button

    private lateinit var editButton: Button

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
                        val currentApp: Commands.App = Commands.App.valueOf(get(1))
                        if (Commands.App.IDLE == currentApp) {
                            if (Commands.System.LOAD == Commands.System.valueOf(get(2))) {
                                val size: Int = get(3).toInt()
                                ledecoratorAppRecyclerViewAdapter.reset()
                                for (index in 0 until size) {
                                    val app = Commands.App.valueOf(get(index + 4))
                                    ledecoratorAppRecyclerViewAdapter.updateApp(app, index)
                                }
                                handler?.apply {
                                    obtainMessage(FRAME_RECEIVED).sendToTarget()
                                }
                            }
                        } else {
                            val active = it.command == currentApp
                            if (it.active != active) {
                                updateRequired = true
                            }
                            it.active = active
                        }
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
                    Action.SAVE -> {
                        handler?.apply {
                            obtainMessage(STOP_EDITING).sendToTarget()
                        }
                        DataFrames.saveAppsFrame(ledecoratorAppRecyclerViewAdapter.enabledApps)
                    }
                    else -> selectedApp.frame
                }
                action = Action.IDLE
            }
        } else {
            throw RuntimeException("$context must implement ${BluetoothServiceHolder::javaClass.name}")
        }
        action = Action.LOAD
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
                visibility = View.GONE
                setOnClickListener {
                    action = Action.SAVE
                }
            }
            cancelButton = view.findViewById<Button>(R.id.ledecoratorAppsCancelButton).apply {
                visibility = View.GONE
                setOnClickListener {
                    stopEditing()
                    action = Action.LOAD
                }
            }
            editButton = view.findViewById<Button>(R.id.ledecoratorAppsEditButton).apply {
                setOnClickListener {
                    startEditing()
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

    private fun startEditing() {
        editable = true
        editButton.visibility = View.GONE
        saveButton.visibility = View.VISIBLE
        cancelButton.visibility = View.VISIBLE
        ledecoratorAppRecyclerViewAdapter.clearSelection()
    }

    private fun stopEditing() {
        editable = false
        editButton.visibility = View.VISIBLE
        saveButton.visibility = View.GONE
        cancelButton.visibility = View.GONE
        ledecoratorAppRecyclerViewAdapter.notifyDataSetChanged()
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            FRAME_RECEIVED -> ledecoratorAppRecyclerViewAdapter.notifyDataSetChanged()
            STOP_EDITING -> stopEditing()
        }
        return true
    }

    inner class LedecoratorAppsRecyclerViewAdapter(appList: List<LedecoratorApp>) :
        ListAdapter<LedecoratorApp, LedecoratorAppsRecyclerViewAdapter.ViewHolder>(DiffCallback()) {

        private val appList: ArrayList<LedecoratorApp> = ArrayList(appList)

        val enabledApps: List<Commands.App>
            get() = appList.filter { it.enabled }.map { it.command }

        private var selectedIndex: Int = -1

        init {
            submitList(this.appList)
        }

        inner class ViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
            val appNameTextView: TextView = view.ledecoratorAppNameTextView
            val appStatusTextView: TextView = view.ledecoratorAppStatusTextView
            val enabledCheckBox: CheckBox = view.ledecoratorAppAppEnabledCheckBox
            val upButton: ImageButton = view.ledecoratorAppUpButton
            val downButton: ImageButton = view.ledecoratorAppDownButton

        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder =
            ViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.fragment_ledecorator_apps_item, parent, false))

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val item = appList[position]
            holder.apply {
                appNameTextView.text = item.name
                appStatusTextView.apply {
                    if (item.active) {
                        text = getString(R.string.ledecorator_app_status_active)
                        visibility = View.VISIBLE
                        holder.view.setBackgroundColor(
                            if (selectedApp == Commands.App.IDLE)
                                if (item.enabled) {
                                    Color.WHITE
                                } else {
                                    Color.GRAY
                                }
                            else
                                Color.GREEN
                        )
                    } else {
                        text = getString(R.string.ledecorator_app_status_active)
                        visibility = View.GONE
                        holder.view.setBackgroundColor(
                            if (item.enabled) {
                                Color.WHITE
                            } else {
                                Color.GRAY
                            }
                        )
                    }
                }
                enabledCheckBox.isEnabled = editable
                enabledCheckBox.setOnCheckedChangeListener(null)
                enabledCheckBox.isChecked = item.enabled
                enabledCheckBox.setOnCheckedChangeListener { _, isChecked ->
                    item.enabled = isChecked
                    notifyItemChanged(position)
                    saveButton.isEnabled = enabledApps.isNotEmpty()
                }

                upButton.apply {
                    visibility = if (editable) View.VISIBLE else View.GONE
                    isEnabled = position != 0
                    setOnClickListener {
                        appList[position] = appList[position - 1].also {
                            appList[position - 1] = appList[position]
                        }
                        notifyItemRangeChanged(position - 1, 2)
                    }
                }
                downButton.apply {
                    visibility = if (editable) View.VISIBLE else View.GONE
                    isEnabled = position != appList.size - 1
                    setOnClickListener {
                        appList[position] = appList[position + 1].also {
                            appList[position + 1] = appList[position]
                        }
                        notifyItemRangeChanged(position, 2)
                    }
                }

                view.setOnLongClickListener {
                    if (!editable && item.enabled) {
                        if (selectedIndex == holder.adapterPosition) {
                            selectedIndex = -1
                            selectedApp = Commands.App.IDLE
                            it.setBackgroundColor(Color.WHITE)
                        }
                    }
                    true
                }

                view.setOnClickListener {
                    if (!editable && item.enabled) {
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

        fun updateApp(app: Commands.App, order: Int) {
            appList.find { it.command == app }?.apply {
                enabled = true
                val index = appList.indexOf(this)
                if (order != index && index != -1) {
                    appList[order] = appList[index].also { appList[index] = appList[order] }
                }
            }
        }

        fun reset() {
            appList.forEach {
                it.enabled = false
                it.active = false
            }
        }

        fun clearSelection() {
            selectedIndex = -1
            selectedApp = Commands.App.IDLE
            notifyDataSetChanged()
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
