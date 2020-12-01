package org.dragberry.ledecorator.apps

import org.dragberry.ledecorator.apps.clock.ClockAppFragment
import org.dragberry.ledecorator.apps.life.LifeGameFragment
import org.dragberry.ledecorator.apps.sandbox.SandboxAppFragment
import org.dragberry.ledecorator.apps.snake.SnakeGameFragment
import org.dragberry.ledecorator.apps.weather.WeatherAppFragment
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.bluetooth.Commands.App.*

object LedecoratorApps {
    val APPS = listOf(
        LedecoratorApp(SNAKE, "Snake Game", { SnakeGameFragment() }),
        LedecoratorApp(WEATHER, "Weather App", { WeatherAppFragment() }),
        LedecoratorApp(LIFE, "Life Game", { LifeGameFragment() }),
        LedecoratorApp(CLOCK, "Clock App", { ClockAppFragment() }),
        LedecoratorApp(SANDBOX, "Sandbox App", { SandboxAppFragment() })
    )
}

class LedecoratorApp(
    val command: Commands.App,
    val name: String,
    val fragment: (() -> AbstractAppFragment),
    var active: Boolean = false
)