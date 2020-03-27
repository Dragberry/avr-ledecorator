package org.dragberry.ledecorator.bluetooth

import java.nio.charset.StandardCharsets

class BleInterchangeFrame {

    companion object {

        @JvmStatic
        val IDLE = "11000000000000000000".toByteArray(StandardCharsets.US_ASCII)
    }
}