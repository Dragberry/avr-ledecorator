package org.dragberry.ledecorator.apps

import android.os.Parcelable
import kotlinx.android.parcel.Parcelize

object LedecoratorApps {
    val APPS = listOf(
        LedecoratorApp('S'.toByte(), "Snake Game"),
        LedecoratorApp('W'.toByte(), "Weather App"),
        LedecoratorApp('L'.toByte(), "Life Game"),
        LedecoratorApp('C'.toByte(), "Clock App")
    )
}

@Parcelize
class LedecoratorApp(
    val code: Byte,
    val name: String,
    var active: Boolean = false,
    var selected: Boolean = false
) : Parcelable