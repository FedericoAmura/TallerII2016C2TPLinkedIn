package com.example.android.clientapp.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;

import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.Modelo.chat.Message;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by emanuel on 11/12/16.
 */

public class PreferenceHandler {

    /** Guarda las credenciales del usuario **/
    public static void saveUserCredentials(UserCredentials credentials, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CREDENTIALS", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putInt("userID", credentials.getUserID());
        editor.putString("token", credentials.getToken());
        editor.commit();
    }

    /** Carga las credenciales del usuario **/
    public static UserCredentials loadUserCredentials(Context context) {
        //SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(context);
        SharedPreferences sharedPref = context.getSharedPreferences("CREDENTIALS", Context.MODE_PRIVATE);
        int userID = sharedPref.getInt("userID", -1);
        String token = sharedPref.getString("token", "");
        return new UserCredentials(userID, token);
    }

    /** Guarda un nuevo mensaje **/
    /** flag
      * 0 : senderID (Me)
      * 1 : receiverID
      * */
    public static void saveMessage(int userID, Message message, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CONVERSATIONS", Context.MODE_PRIVATE);
        String conv = sharedPref.getString(String.valueOf(userID), "");
        String flag = (message.is_mine()) ? "0" : "1";
        SharedPreferences.Editor editor = sharedPref.edit();
        if (conv.isEmpty()) {
            editor.putString(String.valueOf(userID), flag + "-" + message.getMessage() + "-" + message.getHour());
            editor.commit();
            return;
        }
        conv = conv + ";" + flag + "-" + message.getMessage() + "-" + message.getHour();
        editor.putString(String.valueOf(userID), conv);
        editor.commit();
    }

    public static ArrayList<Message> loadMessages(int userID, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CONVERSATIONS", Context.MODE_PRIVATE);
        String conv = sharedPref.getString(String.valueOf(userID), "");
        ArrayList<Message> messages = new ArrayList<Message>();
        if (conv.isEmpty())
            return messages;
        ArrayList<String> msgs = new ArrayList<String>(Arrays.asList(conv.split(";")));
        for (String msg : msgs)
            messages.add(Message.hidrate(msg));
        return messages;
    }

    /** Guarda el último mensaje con otro usuario **/
    public static void saveLastChatMessage(Chat chat, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("LATEST_CHATS", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        String data = chat.getName() + ";" + chat.getLastMessage() + ";" + chat.getHour();
        editor.putString(String.valueOf(chat.getSenderID()), data);
        editor.commit();

        updateSavedConversations(String.valueOf(chat.getSenderID()), context);
    }

    /** Devuelve el último mensaje con otro usuario **/
    private static Chat getSavedLastChat(String senderID, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("LATEST_CHATS", Context.MODE_PRIVATE);
        String dataChat = sharedPref.getString(senderID, "");
        if (dataChat.isEmpty())
            return null;
        return Chat.hidrate(Integer.valueOf(senderID), dataChat);
    }

    /** Devuelve los chats (últimos) de cada usuario **/
    public static ArrayList<Chat> getSavedConversations(Context context) {
        ArrayList<Chat> chats = new ArrayList<Chat>();
        SharedPreferences sharedPref = context.getSharedPreferences("CHATS",Context.MODE_PRIVATE);
        String userIDs = sharedPref.getString("userIDs", "");
        if (userIDs.isEmpty())
            return chats;
        String[] ids = userIDs.split(";");
        for (String id : ids) {
            Chat chat = getSavedLastChat(id, context);
            if (chat != null)
                chats.add(chat);
        }
        return chats;
    }

    /** Actualiza el orden de los últimos chats cuando se agrega otro (puede ser uno ya existente)**/
    private static void updateSavedConversations(String senderID, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CHATS",Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        String userIDs = sharedPref.getString("userIDs", "");
        if (userIDs.isEmpty()){
            editor.putString("userIDs", senderID);
            editor.commit();
            return;
        }
        ArrayList<String> ids = new ArrayList<String>(Arrays.asList(userIDs.split(";")));
        if (ids.contains(senderID))
            ids.remove(senderID);
        ids.add(senderID);
        String stream = TextUtils.join(";", ids);
        editor.putString("userIDs", stream);
        editor.commit();
    }
}