package org.dragberry.ledecorator.bluetooth

import kotlin.math.abs

class BleUtils {

    companion object {
        @JvmStatic
        fun uint8(byte: Byte): Int {
            return if (byte < 0) (Byte.MAX_VALUE.toInt() + 1) * 2 + byte else byte.toInt()
        }

        @JvmStatic
        fun uint16(low: Byte, high: Byte): Int {
            return uint8(low) + uint8(high) * 256
        }

        @JvmStatic
        fun int32(low: Byte, m1: Byte, m2: Byte, high: Byte): Int {
            return (uint8(low) +
                    uint8(m1) * 256 +
                    uint8(m2) * 65536 +
                    abs(high.toInt()) * 16777216) * if (high < 0) -1 else 1
        }

        @JvmStatic
        fun byte0(int: Int): Byte {
            return int.toByte()
        }

        @JvmStatic
        fun byte1(int: Int): Byte {
            return (int shr 8).toByte()
        }
    }

}