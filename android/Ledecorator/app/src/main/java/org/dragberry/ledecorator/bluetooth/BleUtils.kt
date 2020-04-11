package org.dragberry.ledecorator.bluetooth

class BleUtils {

    companion object {
        @JvmStatic
        fun uint8(byte: Byte): Int {
            return if (byte < 0) (Byte.MAX_VALUE.toInt() + 1) * 2 + byte else byte.toInt()
        }

        fun uint16(high: Byte, low: Byte): Int {
            return uint8(high) * 256 + uint8(low)
        }
    }

}