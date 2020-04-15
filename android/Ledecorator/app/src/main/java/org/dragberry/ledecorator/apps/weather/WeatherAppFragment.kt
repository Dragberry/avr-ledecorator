package org.dragberry.ledecorator.apps.weather


import android.graphics.Color
import android.os.Bundle
import android.os.Message
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.RadioGroup
import android.widget.TextView
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.synthetic.main.fragment_weather_app_sensor.view.*

import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands

private const val TAG = "WeatherAppFragment"

private const val UPDATE_SENSORS = 5000

class WeatherAppFragment : AbstractAppFragment() {

    override val fragmentId: String = TAG

    private enum class Mode(code: Char) {
        CAROUSEL('C'),
        VALUE('V'),
        CHARTS('H');

        val code: Byte = code.toByte()
    }

    @Volatile
    private var mode: Mode = Mode.CAROUSEL

    private val sensors: List<Sensor<*>> = listOf(
        TemperatureSensor("Temperature:", "℃"),
        PressureSensor("Pressure:", "mmHg")
    )

    private val sensorRecyclerViewAdapter: SensorRecyclerViewAdapter = SensorRecyclerViewAdapter()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_weather_app, container, false)?.apply {
            findViewById<RecyclerView>(R.id.weatherSensorList)?.apply {
                layoutManager = LinearLayoutManager(context)
                adapter = sensorRecyclerViewAdapter
            }
            findViewById<RadioGroup>(R.id.weatherModeRadioGroup)?.apply {
                check(R.id.weatherModeCarouselRadio)
                setOnCheckedChangeListener { _, checkedId ->
                    when (checkedId) {
                        R.id.weatherModeCarouselRadio -> {
                            mode = Mode.CAROUSEL
                        }
                        R.id.weatherModeValueRadio -> {
                            mode = Mode.VALUE
                        }
                        R.id.weatherModeChartsRadio -> {
                            mode = Mode.CHARTS
                        }
                    }
                }
            }
        }
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        sensors.forEach { it.setValue(bytes) }
        handler?.apply {
            obtainMessage(UPDATE_SENSORS).sendToTarget()
        }
        return ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.WEATHER.code
                2 -> Commands.System.INFINITE.code
                3 -> sensorRecyclerViewAdapter.selectedSensorCode
                4 -> mode.code
                19 -> Commands.Frame.END.code
                else -> 0
            }
        }
    }

    override fun handleMessage(msg: Message): Boolean {
        when (msg.what) {
            UPDATE_SENSORS -> sensorRecyclerViewAdapter.notifyDataSetChanged()
        }
        return true
    }

    inner class SensorRecyclerViewAdapter :
        ListAdapter<Sensor<*>, SensorRecyclerViewAdapter.ViewHolder>(DiffCallback()) {

        private var selectedIndex: Int = -1

        var selectedSensorCode: Byte = IDLE_SENSOR_CODE

        init {
            submitList(sensors)
        }

        inner class ViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
            val nameTextView: TextView = view.weatherSensorNameTextView
            val valueTextView: TextView = view.weatherSensorValueTextView
            val unitTextView: TextView = view.weatherSensorUnitTextView
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder =
            ViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.fragment_weather_app_sensor, parent, false))

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val item = sensors[position]
            holder.apply {
                nameTextView.text = item.name
                valueTextView.text = item.value.toString()
                unitTextView.text = item.unit

                view.setBackgroundColor(
                    if (selectedIndex == holder.adapterPosition)
                        Color.GREEN
                    else
                        Color.WHITE
                )
                view.setOnClickListener {
                    if (selectedIndex != holder.adapterPosition) {
                        selectedIndex = holder.adapterPosition
                        selectedSensorCode = item.code
                    } else {
                        selectedIndex = -1
                        selectedSensorCode = IDLE_SENSOR_CODE
                    }
                    notifyItemChanged(selectedIndex)
                }
            }

        }
    }

    private class DiffCallback : DiffUtil.ItemCallback<Sensor<*>>() {

        override fun areItemsTheSame(oldItem: Sensor<*>, newItem: Sensor<*>): Boolean {
            return oldItem.name == newItem.name
        }

        override fun areContentsTheSame(oldItem: Sensor<*>, newItem: Sensor<*>): Boolean {
            return oldItem.name == newItem.name
        }
    }

    companion object {

        @JvmStatic
        private val IDLE_SENSOR_CODE: Byte = 'I'.toByte()
    }
}
