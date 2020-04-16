package org.dragberry.ledecorator.bluetooth

import java.lang.StringBuilder

class Commands {
    enum class App(code: Char, private val logger: (sb: StringBuilder, data: ByteArray) -> Unit) {
        IDLE('I', { _, _ -> }),
        SNAKE('S', { sb , data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
            sb.append("\tHead:\t[X=${data[4]};Y=${data[5]}]\n")
            sb.append("\tTail:\t[X=${data[6]};Y=${data[7]}]\n")
            sb.append("\tSpeed:\t${data[8]}")
        }),
        WEATHER('W', { sb , data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
            sb.append("\tTemperature:\t${BleUtils.int32(data[4], data[5], data[6], data[7]) / 100.0} C\n")
            sb.append("\tPressure:\t${BleUtils.int32(data[8], data[9], data[10], data[11]) / 133} mmHg")

        }),
        CLOCK('C', { sb , data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
            sb.append("\t${data[4]}:${data[5]}:${data[6]} ${data[7]}/${data[8]}/${data[9]}")

        }),
        LIFE('L', { sb , data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
            sb.append("\tSteps:\t${BleUtils.uint16(data[4], data[5])}\n")
            sb.append("\tColor Live:\t${data[6]}\n")
            sb.append("\tColor Dead:\t${data[7]}\n")
            sb.append("\tRandom:\t${data[8].toInt() != 0}\n")
        });

        val code: Byte = code.toByte()

        fun toString(dataFrame: ByteArray): String {
            return StringBuilder().apply {
                append(name).append("\n")
                logger.invoke(this, dataFrame)
            }.toString()
        }

        companion object {
            fun valueOf(code: Byte): App = values().find { it.code == code } ?: IDLE
        }
    }

    enum class Frame(code: Int) {
        START(1),
        END(4);

        val code: Byte = code.toByte()
    }

    enum class System(code: Char) {
        INFINITE('I'),
        FINITE('F'),
        RESTART('R');

        val code: Byte = code.toByte()
    }
}

class DataFrames {

    companion object {
        fun check(bytes: ByteArray): Boolean {
            return Commands.Frame.START.code == bytes[0] && Commands.Frame.END.code == bytes[19]
        }

        @JvmStatic
        val IDLE = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.IDLE.code
                2 -> Commands.System.FINITE.code
                19 -> Commands.Frame.END.code
                else -> 0
            }.toByte()
        }
    }
}