package org.dragberry.ledecorator.apps.weather

import org.dragberry.ledecorator.bluetooth.BleUtils

abstract class Sensor<T : Number>(val name: String, val unit: String) {
    lateinit var value: T
        protected set

    abstract fun setValue(data: ByteArray)
}

class TemperatureSensor(name: String, unit: String) : Sensor<Double>(name, unit) {
    init {
        value = 0.0
    }

    override fun setValue(data: ByteArray) {
        value = BleUtils.int32(data[4], data[5], data[6], data[7]) / 100.0
    }
}

class PressureSensor(name: String, unit: String) : Sensor<Int>(name, unit) {
    init {
        value = 0
    }

    override fun setValue(data: ByteArray) {
        value = BleUtils.int32(data[8], data[9], data[10], data[11]) / 133
    }
}


