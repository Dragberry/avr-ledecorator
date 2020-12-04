package org.dragberry.ledecorator.utils

import android.app.AlertDialog
import android.app.Dialog
import android.content.Context
import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import android.widget.GridView
import androidx.fragment.app.DialogFragment
import org.dragberry.ledecorator.R

class SelectColorDialogFragment(private val onColorSelected: (color: Color) -> Unit) : DialogFragment() {

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return activity?.let {
            val builder = AlertDialog.Builder(it)
            val inflater = requireActivity().layoutInflater
            val view: GridView = inflater.inflate(R.layout.fragment_select_color, null) as GridView
            view.adapter = ColorsAdapter(context)
            builder.setView(view)
            builder.create()
        } ?: throw IllegalStateException("Activity cannot be null")
    }

    inner class ColorsAdapter(private var context: Context?) : BaseAdapter() {

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            return Button(context).apply {
                val color: Color = Colors.ALL[position]
                setBackgroundColor(color.real)
                setOnClickListener {
                    onColorSelected(color)
                    this@SelectColorDialogFragment.dismiss()
                }
            }
        }

        override fun getItem(position: Int): Any? = null

        override fun getItemId(position: Int): Long = 0

        override fun getCount(): Int = 64



    }
}


