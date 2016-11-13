package com.example.android.clientapp.ArrayAdapters;

import android.content.Context;
import android.media.Image;
import android.provider.ContactsContract;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.R;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

/**
 * Created by emanuel on 11/12/16.
 */

public class ChatArrayAdapter extends ArrayAdapter<Chat> {
    private Context context;
    private List<Chat> chatList = new ArrayList<Chat>();

    public ChatArrayAdapter(Context context, int resource) {
        super(context, resource);
        this.context = context;
    }

    @Override
    public void add(Chat object) {
        chatList.add(object);
        super.add(object);
    }

    public int getCount() {
        return chatList.size();
    }

    public Chat getItem(int index) {
        return chatList.get(index);
    }

    private class ViewHolder {
        ImageView img_photo;
        TextView t_name;
        TextView t_last_msg;
        TextView t_hour;
    }

    public View getView(int index, View convertView, ViewGroup parent) {
        ViewHolder holder = null;
        View row = convertView;
        LayoutInflater inflater = (LayoutInflater) this.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        if (row == null) {
            row = inflater.inflate(R.layout.chat_item, null);
            holder = new ViewHolder();

            holder.t_name = (TextView) row.findViewById(R.id.person_name);
            holder.t_last_msg = (TextView) row.findViewById(R.id.last_msg);
            holder.t_hour = (TextView) row.findViewById(R.id.hour);
            holder.img_photo = (ImageView) row.findViewById(R.id.profile_picture);
            Chat chat = getItem(index);
            holder.t_name.setText(chat.getName());
            holder.t_last_msg.setText(chat.getLastMessage());
            holder.t_hour.setText(chat.getHour());
            holder.img_photo.setImageResource(R.drawable.ic_user_black);

            row.setTag(holder);

        } else
            holder = (ViewHolder) row.getTag();
        return row;
    }
}
