package org.dragberry.ledecorator.apps

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import org.dragberry.ledecorator.R

class LedecoratorAppFragment : Fragment() {

    private var listener: LedecoratorAppInteractionListener? = null

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_ledecoratorapp_list, container, false)
        if (view is RecyclerView) {
            with(view) {
                layoutManager = LinearLayoutManager(context)
                adapter = LedecoratorAppRecyclerViewAdapter(LedecoratorApps.APPS, listener)
            }
        }
        return view
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is LedecoratorAppInteractionListener) {
            listener = context
        } else {
            throw RuntimeException("$context must implement LedecoratorAppInteractionListener")
        }
    }

    override fun onDetach() {
        super.onDetach()
        listener = null
    }

    interface LedecoratorAppInteractionListener {
        fun onLedecoratorAppInteraction(app: LedecoratorApp?)
    }

    companion object {

        @JvmStatic
        fun newInstance() = LedecoratorAppFragment()
    }
}
