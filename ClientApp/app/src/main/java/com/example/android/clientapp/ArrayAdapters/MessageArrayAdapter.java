package com.example.android.clientapp.ArrayAdapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.example.android.clientapp.Modelo.chat.Message;
import com.example.android.clientapp.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by emanuel on 11/12/16.
 */

public class MessageArrayAdapter extends ArrayAdapter<Message> {
    private Context context;
    private TextView textView_msg;
    private TextView textView_timestamp;
    private List<Message> msgList = new ArrayList<Message>();

    public MessageArrayAdapter(Context context, int resource) {
        super(context, resource);
        this.context = context;
    }

    @Override
    public void add(Message object) {
        msgList.add(object);
        super.add(object);
    }

    public int getCount() {
        return msgList.size();
    }

    public Message getItem(int index) {
        return msgList.get(index);
    }

    public View getView(int index, View convertView, ViewGroup parent) {
        Message msg = getItem(index);
        View row;
        LayoutInflater inflater = (LayoutInflater) this.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        if (msg.is_mine())
            row = inflater.inflate(R.layout.right_message, parent, false);
        else
            row = inflater.inflate(R.layout.left_message, parent, false);
        textView_msg = (TextView) row.findViewById(R.id.msg);
        textView_msg.setText(msg.getMessage());
        textView_timestamp = (TextView) row.findViewById(R.id.timestamp);
        textView_timestamp.setText(msg.getHour());
        return row;
    }

}
