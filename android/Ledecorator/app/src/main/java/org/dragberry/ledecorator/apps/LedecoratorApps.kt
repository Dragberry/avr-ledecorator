package org.dragberry.ledecorator.apps

import android.os.Parcelable
import kotlinx.android.parcel.Parcelize
import org.dragberry.ledecorator.bluetooth.Commands

object LedecoratorApps {
    val APPS = listOf(
        LedecoratorApp(Commands.App.SNAKE, "Snake Game"),
        LedecoratorApp(Commands.App.WEATHER, "Weather App"),
        LedecoratorApp(Commands.App.LIFE, "Life Game"),
        LedecoratorApp(Commands.App.CLOCK, "Clock App")
    )

}

@Parcelize
class LedecoratorApp(
    val command: Commands.App,
    val name: String,
    var active: Boolean = false,
    var selected: Boolean = false
) : Parcelable