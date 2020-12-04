package org.dragberry.ledecorator.utils


class Color(val display: Int, val real: Int)

object Colors {
    val ALL:  Array<Color> = Array(64) { i ->
        Color(i, convertColor(i))
    }

    val BLACK = ALL[0]
    val WHITE = ALL[63]

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
}