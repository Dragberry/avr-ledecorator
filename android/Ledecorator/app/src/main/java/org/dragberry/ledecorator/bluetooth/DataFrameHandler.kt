package org.dragberry.ledecorator.bluetooth

interface DataFrameHandler {
    fun handlerDateFrame(frame: ByteArray): ByteArray
}