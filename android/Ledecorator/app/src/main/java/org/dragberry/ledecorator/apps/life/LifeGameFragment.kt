package org.dragberry.ledecorator.apps.life


import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.bluetooth.DataFrames

private const val TAG = "LifeGameFragment"

class LifeGameFragment : AbstractAppFragment(TAG) {

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_life_game, container, false)
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return Commands.App.LIFE.frame
    }

}
