package org.dragberry.ledecorator.bluetooth

class BleInterchangeFrame {

    companion object {

        @JvmStatic
        val FRAME_START = 1.toByte()

        @JvmStatic
        val FRAME_END = 4.toByte()

        @JvmStatic
        val APP_IDLE = 'I'.toByte()

        @JvmStatic
        val APP_SNAKE = 'S'.toByte()

        @JvmStatic
        val COMMAND_IDLE = 'I'.toByte()

        @JvmStatic
        val COMMAND_RESTART = 'R'.toByte()

        @JvmStatic
        val IDLE = ByteArray(20) {
            when (it) {
                0 -> FRAME_START
                1 -> APP_IDLE
                2 -> COMMAND_IDLE
                19 -> FRAME_END
                else -> 0
            }.toByte()
        }
    }
}