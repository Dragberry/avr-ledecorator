package org.dragberry.ledecorator.apps

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import org.dragberry.ledecorator.R


import org.dragberry.ledecorator.apps.LedecoratorAppFragment.LedecoratorAppInteractionListener

import kotlinx.android.synthetic.main.fragment_ledecoratorapp.view.*

class LedecoratorAppRecyclerViewAdapter(
    private val mValues: List<LedecoratorApp>,
    private val mListener: LedecoratorAppInteractionListener?
) : RecyclerView.Adapter<LedecoratorAppRecyclerViewAdapter.ViewHolder>() {

    private val mOnClickListener: View.OnClickListener

    init {
        mOnClickListener = View.OnClickListener {
            mListener?.onLedecoratorAppInteraction(it.tag as LedecoratorApp)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_ledecoratorapp, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = mValues[position]
        holder.mIdView.text = item.code.toString(16)
        holder.mContentView.text = item.name

        with(holder.mView) {
            tag = item
            setOnClickListener(mOnClickListener)
        }
    }

    override fun getItemCount(): Int = mValues.size

    inner class ViewHolder(val mView: View) : RecyclerView.ViewHolder(mView) {
        val mIdView: TextView = mView.item_number
        val mContentView: TextView = mView.content
    }
}
