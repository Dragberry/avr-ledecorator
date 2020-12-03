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

class SelectColorDialogFragment(private val listener: SelectColorDialogListener) : DialogFragment() {

    interface SelectColorDialogListener {
        fun onColorSelected(color: Pair<Int, Int>)
    }

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

        private val colors: IntArray = IntArray(64) { i -> i}

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            return Button(context).apply {
                val colorDisplay: Int = colors[position]
                val color: Int = convertColor(colorDisplay)
                setBackgroundColor(color)
                setOnClickListener {
                    listener.onColorSelected(color to colorDisplay)
                    this@SelectColorDialogFragment.dismiss()
                }
            }
        }

        override fun getItem(position: Int): Any? = null

        override fun getItemId(position: Int): Long = 0

        override fun getCount(): Int = 64

        private fun convertColor(color: Int): Int {
            val red = when (color and 0b00000011) {
                1 -> 0x00550000
                2 -> 0x00AA0000
                3 -> 0x00FF0000
                else -> 0
            }
            val green = when(color and 0b00001100 shr 2) {
                1 -> 0x00005500
                2 -> 0x0000AA00
                3 -> 0x0000FF00
                else -> 0
            }
            val blue = when(color and 0b00110000 shr 4) {
                1 -> 0x00000055
                2 -> 0x000000AA
                3 -> 0x000000FF
                else -> 0
            }
            return 0xFF000000.toInt() or red or green or blue
        }

    }
}


