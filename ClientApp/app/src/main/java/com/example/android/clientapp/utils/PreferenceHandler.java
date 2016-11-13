package com.example.android.clientapp.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import com.example.android.clientapp.Modelo.chat.Chat;

import java.util.ArrayList;

/**
 * Created by emanuel on 11/12/16.
 */

public class PreferenceHandler {

    public static UserCredentials loadUserCredentials(Context context) {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(context);
        int userID = sharedPref.getInt("userID", -1);
        String token = sharedPref.getString("token", "");
        return new UserCredentials(userID, token);
    }

    public static void saveLastChatMessage(Chat chat, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("chats_id", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        String data = chat.getName() + ";" + chat.getLastMessage() + ";" + chat.getHour();
        editor.putString(String.valueOf(chat.getSenderID()), data);
        editor.commit();
    }

    public static Chat getSavedChat(String senderID, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("chats_id", Context.MODE_PRIVATE);
        String dataChat = sharedPref.getString(senderID, "");
        if (dataChat.isEmpty())
            return null;
        String[] data = dataChat.split(";");
        String name = data[0];
        String last_msg = data[1];
        String hour = data[2];
        return new Chat(Integer.valueOf(senderID), name, last_msg);
    }

    public static ArrayList<Chat> getSavedConversations(Context context) {
        ArrayList<Chat> chats = new ArrayList<Chat>();
        SharedPreferences sharedPref = context.getSharedPreferences("chats",Context.MODE_PRIVATE);
        String userIDs = sharedPref.getString("userIDs", "");
        if (userIDs.isEmpty())
            return chats;
        String[] ids = userIDs.split(";");
        for (String id : ids) {
            Chat chat = getSavedChat(id, context);
            if (chat != null)
                chats.add(chat);
        }
        return chats;
    }
}
