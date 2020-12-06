package org.dragberry.ledecorator.apps.life


import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Spinner
import org.dragberry.ledecorator.R
import org.dragberry.ledecorator.apps.AbstractAppFragment
import org.dragberry.ledecorator.bluetooth.Commands
import org.dragberry.ledecorator.utils.SelectColorDialogFragment

private const val TAG = "LifeGameFragment"

class LifeGameFragment : AbstractAppFragment(TAG) {

    private lateinit var liveColorButton: Button
    private lateinit var deadColorButton: Button
    private lateinit var modeSelectorSpinner: Spinner


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_life_game, container, false)
        return view?.apply {
            liveColorButton = findViewById<Button>(R.id.lifeGameLiveColorButton).apply {
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            liveColorButton.setBackgroundColor(it.real)
                        }.show(this, "selectColor")
                    }
                }
            }
            deadColorButton = findViewById<Button>(R.id.lifeGameDeadColorButton).apply {
                setOnClickListener {
                    fragmentManager?.apply {
                        SelectColorDialogFragment {
                            deadColorButton.setBackgroundColor(it.real)
                        }.show(this, "selectColor")
                    }
                }
            }
            modeSelectorSpinner = findViewById<Spinner>(R.id.lifeGameModeSelector).apply {
//                adapter = object ArrayAdapter<String> {

//                }

            }
        }
    }

    override fun onDataFrame(bytes: ByteArray): ByteArray {
        return Commands.App.LIFE.frame
    }


}
