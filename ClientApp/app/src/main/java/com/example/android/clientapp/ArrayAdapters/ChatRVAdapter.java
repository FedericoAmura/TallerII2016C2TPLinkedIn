package com.example.android.clientapp.ArrayAdapters;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.android.clientapp.ChatActivity;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.R;
import com.example.android.clientapp.utils.PreferenceHandler;

import java.util.ArrayList;
import java.util.List;

import static android.support.v7.widget.RecyclerView.*;

/**
 * Created by emanuel on 11/12/16.
 */

public class ChatRVAdapter extends RecyclerView.Adapter<ChatRVAdapter.ChatViewHolder> {
    private List<Chat> chatList = new ArrayList<Chat>();
    private Context context;

    public static class ChatViewHolder extends ViewHolder{
        CardView cardView;
        ImageView img_photo;
        TextView t_name;
        TextView t_last_msg;
        TextView t_hour;
        int receiverID;

        ChatViewHolder(View view) {
            super(view);
            cardView = (CardView) view.findViewById(R.id.cv);
            img_photo = (ImageView) view.findViewById(R.id.profile_picture);
            t_name = (TextView) view.findViewById(R.id.person_name);
            t_last_msg = (TextView) view.findViewById(R.id.last_msg);
            t_hour = (TextView) view.findViewById(R.id.hour);

            view.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Intent intent = new Intent(view.getContext(), ChatActivity.class);
                    intent.putExtra("name", t_name.getText().toString());
                    intent.putExtra("receiverID", receiverID);
                    view.getContext().startActivity(intent);
                }
            });
        }
    }

    public ChatRVAdapter(Context context) {
        this.context = context;
    }

    public void add(Chat chat) {
        if (chatList.contains(chat)) {
            int index = chatList.indexOf(chat);
            chatList.remove(index);
        }
        chatList.add(0, chat);
        notifyDataSetChanged();
    }

    @Override
    public void onAttachedToRecyclerView(RecyclerView recyclerView) {
        super.onAttachedToRecyclerView(recyclerView);
    }

    @Override
    public ChatViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.chat_item, parent, false);
        ChatViewHolder chatViewHolder = new ChatViewHolder(view);
        return chatViewHolder;
    }

    @Override
    public void onBindViewHolder(ChatViewHolder holder, int position) {
        Chat chat = chatList.get(position);
        Bitmap thumb = PreferenceHandler.getUserThumbnail(chat.getReceiverID(), context);
        if (thumb != null)
            holder.img_photo.setImageBitmap(thumb);
        else
            holder.img_photo.setImageResource(R.drawable.ic_user_black);
        holder.t_name.setText(chat.getName());
        String message = chat.getLastMessage();
        if (message.length() > 20)
            holder.t_last_msg.setText(message.substring(0,20) + "...");
        else
            holder.t_last_msg.setText(message);
        holder.t_hour.setText(chat.getHour());
        holder.receiverID = chat.getReceiverID();
    }

    @Override
    public int getItemCount() {
        return chatList.size();
    }
}
