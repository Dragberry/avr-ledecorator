package org.dragberry.ledecorator.apps

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import kotlinx.android.synthetic.main.fragment_ledecoratorapp.view.*
import org.dragberry.ledecorator.R


class LedecoratorAppRecyclerViewAdapter(
    private val appList: List<LedecoratorApp>
) :
    RecyclerView.Adapter<LedecoratorAppRecyclerViewAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_ledecoratorapp, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = appList[position]
        holder.appNameTextView.text = item.name
        holder.appCodeTextView.text = "0x0${item.code.toString(16)}"
        holder.appStatusTextView.apply {
            if (item.active) {
                text = "Active"
                visibility = View.VISIBLE
            } else {
                text = "Inactive"
                visibility = View.INVISIBLE
            }
        }

        with(holder.view) {
            tag = item
//            setOnClickListener(mOnClickListener)
        }
    }

    override fun getItemCount(): Int = appList.size

    inner class ViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
        val appNameTextView: TextView = view.ledecoratorAppNameTextView
        val appCodeTextView: TextView = view.ledecoratorAppCodeTextView
        val appStatusTextView: TextView = view.ledecoratorAppStatusTextView
    }
}
