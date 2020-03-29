package org.dragberry.ledecorator.bluetooth

import java.nio.charset.StandardCharsets

class BleInterchangeFrame {

    companion object {

        @JvmStatic
        val IDLE = "11111111111111111111".toByteArray(StandardCharsets.US_ASCII)
    }
}