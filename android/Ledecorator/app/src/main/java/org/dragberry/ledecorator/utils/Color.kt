package org.dragberry.ledecorator.utils


class Color(val display: Int, val real: Int)

object Colors {
    private val ALL_DISPLAY:  Array<Color> = Array(64) { i ->
        Color(i, convertColor(i))
    }
    private val ALL_REAL: Map<Int, Color> = ALL_DISPLAY.associateBy { it.real }


    val BLACK = ALL_DISPLAY[0]
    val WHITE = ALL_DISPLAY[63]

    fun getByDisplay(value: Int): Color = ALL_DISPLAY[value]

    fun getByReal(value: Int): Color = ALL_REAL.getOrDefault(compressColor(value), BLACK)

    fun convertColor(color: Int): Int {
        val red = when (color and 0b00000011) {
            1 -> 0x00550000
            2 -> 0x00AA0000
            3 -> 0x00FF0000
            else -> 0
        }
        val green = when(color and 0b00001100 shr 2) {
            1 -> 0x00005500
            2 -> 0x0000AA00
            3 -> 0x0000FF00
            else -> 0
        }
        val blue = when(color and 0b00110000 shr 4) {
            1 -> 0x00000055
            2 -> 0x000000AA
            3 -> 0x000000FF
            else -> 0
        }
        return 0xFF000000.toInt() or red or green or blue
    }

    fun compressColor(color: Int): Int {
        val red = color and 0x00FF0000 shr 16
        val redD = when {
            red == 0 -> 0
            red <= 0x55 -> 0x00550000
            red <= 0xAA -> 0x00AA0000
            else -> 0x00FF0000

        }
        val green = color and 0x0000FF00 shr 8
        val greenD = when {
            green == 0 -> 0
            green <= 0x55 -> 0x00005500
            green <= 0xAA -> 0x0000AA00
            else -> 0x0000FF00

        }
        val blue = color and 0x000000FF
        val blueD = when {
            blue == 0 -> 0
            blue <= 0x55 -> 0x00000055
            blue <= 0xAA -> 0x000000AA
            else -> 0x000000FF

        }
        return 0xFF000000.toInt() or redD or greenD or blueD
    }
}