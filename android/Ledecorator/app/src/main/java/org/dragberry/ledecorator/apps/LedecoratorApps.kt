package org.dragberry.ledecorator.apps

import android.os.Parcelable
import kotlinx.android.parcel.Parcelize

object LedecoratorApps {
    val APPS = listOf(
        LedecoratorApp(1, "Snake Game"),
        LedecoratorApp(2, "Weather App"),
        LedecoratorApp(3, "Life Game"),
        LedecoratorApp(4, "Clock App")
    )
}

@Parcelize
class LedecoratorApp(
    val code: Byte,
    val name: String,
    var active: Boolean = false
) : Parcelable