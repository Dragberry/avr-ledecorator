package org.dragberry.ledecorator.apps.sandbox

private const val UPLOAD_FRAME_SIZE = 12

enum class Action(val value: Byte) {
    LOAD_PICTURE('L'.toByte()),
    DRAW_POINT('P'.toByte()),
    DRAW_BLOCK('B'.toByte()),
    CLEAR_SCREEN('C'.toByte()),
    SAVE_PICTURE('S'.toByte())
}

sealed class Command(val code: Action)

class DrawPointCommand(val x: Int, val y: Int, val color: Int, val previousColor: Int) :
    Command(Action.DRAW_POINT) {

    override fun equals(other: Any?): Boolean {
        return other != null && other is DrawPointCommand &&
                x == other.x && y == other.y && color == other.color
    }

    override fun hashCode(): Int {
        return x + y + color
    }

}

class DrawBlockCommand(val x: Int, val y: Int, val size: Int, val block: IntArray) :
    Command(Action.DRAW_BLOCK) {

    override fun equals(other: Any?): Boolean {
        return other != null && other is DrawBlockCommand &&
                x == other.x && y == other.y && size == other.size && block.contentEquals(other.block)
    }

    override fun hashCode(): Int {
        return x + y + size + block.hashCode()
    }

}

class ClearScreenCommand(val color: Int) : Command(Action.CLEAR_SCREEN) {

    override fun equals(other: Any?): Boolean {
        return other != null && other is ClearScreenCommand && color == other.color
    }

    override fun hashCode(): Int {
        return color
    }

}

class SavePictureCommand : Command(Action.SAVE_PICTURE) {

    override fun equals(other: Any?): Boolean {
        return other != null && other is SavePictureCommand
    }

    override fun hashCode(): Int {
        return 0
    }

}

class LoadPictureCommand(val x: Int, val y: Int, val size: Int = UPLOAD_FRAME_SIZE) :
    Command(Action.LOAD_PICTURE) {

    override fun equals(other: Any?): Boolean {
        return other != null && other is LoadPictureCommand && x == other.x && y == other.y && size == other.size
    }

    override fun hashCode(): Int {
        return 0
    }

}