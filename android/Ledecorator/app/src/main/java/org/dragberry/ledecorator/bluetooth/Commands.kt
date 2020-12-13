package org.dragberry.ledecorator.bluetooth

import java.lang.StringBuilder
import org.dragberry.ledecorator.apps.life.LifeGameFragment
import org.dragberry.ledecorator.apps.snake.SnakeGameFragment

class Commands {
    enum class App(code: Char, private val logger: (sb: StringBuilder, data: ByteArray) -> Unit) {
        IDLE('I', { _, _ -> }),
        SNAKE('S', { sb , data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
            when (data[4]) {
                System.LOAD.code -> {
                    sb.append("\tTTL:\t${BleUtils.uint16(data[5], data[6])}\n")
                    sb.append("\tField color:\t${data[7]}\n")
                    sb.append("\tHead color:\t${data[8]}\n")
                    sb.append("\tBody color:\t${data[9]}\n")
                    sb.append("\tDead color:\t${data[10]}\n")
                    sb.append("\tSpeed:\t${data[11]}\n")
                }
                System.EMPTY.code -> {
                    sb.append("\tMode:\t${SnakeGameFragment.Mode.valueOf(data[5])}\n")
                    sb.append("\tHead:\t[X=${data[6]};Y=${data[7]}]\n")
                    sb.append("\tTail:\t[X=${data[8]};Y=${data[9]}]\n")
                    sb.append("\tSpeed:\t${data[10]}")
                }
            }
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
            when (data[4]) {
                System.LOAD.code -> {
                    sb.append("\tTTL:\t${BleUtils.uint16(data[5], data[6])}\n")
                    sb.append("\tLife color:\t${data[7]}\n")
                    sb.append("\tDead color:\t${data[8]}\n")
                    sb.append("\tMode:\t${data[9]}\n")
                    sb.append("\tScript:\t${data[10]}\n")
                    sb.append("\tSpeed:\t${data[11]}\n")
                }
                System.EMPTY.code -> {
                    sb.append("\tLife color:\t${data[5]}\n")
                    sb.append("\tDead color:\t${data[6]}\n")
                    sb.append("\tMode:\t${LifeGameFragment.Mode.valueOf(data[7])}\n")
                    sb.append("\tScript:\t${LifeGameFragment.Script.valueOf(data[8])}\n")
                    sb.append("\tSpeed:\t${data[9]}\n")
                    sb.append("\tSteps:\t${BleUtils.uint16(data[10], data[11])}\n")
                }
            }
        }),
        SANDBOX('B', {sb, data ->
            sb.append("\tTime:\t${BleUtils.uint16(data[2], data[3])}\n")
        });

        val code: Byte = code.toByte()

        val frame: ByteArray = ByteArray(20) {
            when (it) {
                0 -> Frame.START.code
                1 -> code.toByte()
                2 -> (if (this.code == 'I'.toByte()) System.FINITE else System.INFINITE).code
                19 -> Frame.END.code
                else -> 0.toByte()
            }
        }

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
        RESTART('R'),
        LOAD('L'),
        SAVE('S'),
        EMPTY('E');

        val code: Byte = code.toByte()

        companion object {
            fun valueOf(code: Byte): System = values().find { it.code == code } ?: FINITE
        }
    }
}

class DataFrames {

    companion object {
        fun check(bytes: ByteArray): Boolean {
            return Commands.Frame.START.code == bytes[0] && Commands.Frame.END.code == bytes[19]
        }

        val loadAppsFrame: ByteArray = ByteArray(20) {
            when (it) {
                0 -> Commands.Frame.START.code
                1 -> Commands.App.IDLE.code
                2 -> Commands.System.LOAD.code
                19 -> Commands.Frame.END.code
                else -> 0.toByte()
            }
        }

        fun saveAppsFrame(apps: List<Commands.App>): ByteArray {
            return ByteArray(20) {
                when (it) {
                    0 -> Commands.Frame.START.code
                    1 -> Commands.App.IDLE.code
                    2 -> Commands.System.SAVE.code
                    3 -> apps.size.toByte()
                    in 4 until 4 + apps.size -> apps[it - 4].code
                    19 -> Commands.Frame.END.code
                    else -> 0.toByte()
                }
            }
        }
    }

}