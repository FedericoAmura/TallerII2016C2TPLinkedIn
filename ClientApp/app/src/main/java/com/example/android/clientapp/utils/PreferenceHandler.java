package com.example.android.clientapp.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;

import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.Modelo.chat.Message;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by emanuel on 11/12/16.
 */

public class PreferenceHandler {

    /** Guarda las credenciales del usuario **/
    public static void saveUserCredentials(UserCredentials credentials, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CREDENTIALS", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putString("username", credentials.getUsername());
        editor.putString("password", credentials.getPassword());
        editor.putInt("userID", credentials.getUserID());
        editor.putString("token", credentials.getToken());
        editor.commit();
    }

    /** Carga las credenciales del usuario **/
    public static UserCredentials loadUserCredentials(Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CREDENTIALS", MODE_PRIVATE);
        int userID = sharedPref.getInt("userID", -1);
        if (userID == -1)
            return null;
        String token = sharedPref.getString("token", "");
        String username = sharedPref.getString("username", "");
        String password = sharedPref.getString("password", "");
        return new UserCredentials(username, password, userID, token);
    }

    /** Borrando las credenciales **/
    public static void removeCredentials(Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("CREDENTIALS", MODE_PRIVATE);
        sharedPref.edit().clear().commit();
    }

    /** Actualiza el token recibido de Firebase Cloud Messaging **/
    public static void updateFCMToken(Context context, String token) {
        SharedPreferences sharedPref = context.getSharedPreferences("FCM_TOKEN", MODE_PRIVATE);
        sharedPref.edit().putString("token", token).commit();
    }

    /** Devuelve el token recibido de Firebase Cloud Messaging **/
    public static String getFCMToken(Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("FCM_TOKEN", MODE_PRIVATE);
        String token = sharedPref.getString("token", null);
        return token;
    }

    /** Guarda un nuevo mensaje **/
    /** flag
      * 0 : senderID (Me)
      * 1 : receiverID
      * */
    public static void saveMessage(int userID, Message message, Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return;
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-CONVERSATIONS", MODE_PRIVATE);
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
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return new ArrayList<>();
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-CONVERSATIONS", MODE_PRIVATE);
        String conv = sharedPref.getString(String.valueOf(userID), "");
        ArrayList<Message> messages = new ArrayList<Message>();
        if (conv.isEmpty())
            return messages;
        ArrayList<String> msgs = new ArrayList<String>(Arrays.asList(conv.split(";")));
        for (String msg : msgs)
            messages.add(Message.hydrate(msg));
        return messages;
    }

    /** Guarda el último mensaje con otro usuario **/
    public static void saveLastChatMessage(Chat chat, Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return;
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-LATEST_CHATS", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        String data = chat.getName() + ";" + chat.getLastMessage() + ";" + chat.getHour();
        editor.putString(String.valueOf(chat.getReceiverID()), data);
        editor.commit();

        updateSavedConversations(String.valueOf(chat.getReceiverID()), context);
    }

    /** Devuelve el último mensaje con otro usuario **/
    private static Chat getSavedLastChat(String senderID, Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return null;
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-LATEST_CHATS", MODE_PRIVATE);
        String dataChat = sharedPref.getString(senderID, "");
        if (dataChat.isEmpty())
            return null;
        return Chat.hydrate(Integer.valueOf(senderID), dataChat);
    }

    /** Devuelve los chats (últimos) de cada usuario **/
    public static ArrayList<Chat> getSavedConversations(Context context) {
        ArrayList<Chat> chats = new ArrayList<Chat>();
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return chats;
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-CHATS", MODE_PRIVATE);
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
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return;
        String myUserID = String.valueOf(credentials.getUserID());
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-CHATS", MODE_PRIVATE);
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

    /** Guarda (y actualiza) el thumbnail de un contacto **/
    public static void updateUserThumbnail(int userID, String thumbnailencoded, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("THUMBNAILS", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putString(String.valueOf(userID), thumbnailencoded);
        editor.commit();
    }

    /** Devuelve el thumbnail de un contacto **/
    public static Bitmap getUserThumbnail(int userID, Context context) {
        SharedPreferences sharedPref = context.getSharedPreferences("THUMBNAILS", MODE_PRIVATE);
        String thumbnailencoded = sharedPref.getString(String.valueOf(userID), null);
        if (thumbnailencoded == null)
            return null;
        byte[] decodedString = Base64.decode(thumbnailencoded, Base64.NO_WRAP);
        InputStream is = new ByteArrayInputStream(decodedString);
        Bitmap thumb = BitmapFactory.decodeStream(is);
        Bitmap circleThumb = CircleBitmap.generate(thumb);
        return circleThumb;
    }

    /** Guarda una nueva notificación **/
    public static void saveNotification(AppServerNotification notification, Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return;
        String myUserID = String.valueOf(credentials.getUserID());
        String senderID = String.valueOf(notification.getSenderID());
        SharedPreferences sharedPref;
        if (notification.getType() == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) {
            sharedPref = context.getSharedPreferences(myUserID + "-MESSAGE_NOTIFICATIONS", MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPref.edit();
            SharedPreferences sharedPrefUsernames = context.getSharedPreferences(myUserID + "-USERNAMES_FROM_MESSAGE_NOTIFICATIONS", MODE_PRIVATE);
            String messagesFromSender = sharedPref.getString(senderID, null);
            if (messagesFromSender == null)
                editor.putString(senderID, notification.getMessage()).commit();
            else
                editor.putString(senderID, messagesFromSender + ";" + notification.getMessage()).commit();
            SharedPreferences.Editor editorUsernames = sharedPrefUsernames.edit();
            editorUsernames.putString(senderID, notification.getUsername()).commit();
        } else {
            sharedPref = context.getSharedPreferences(myUserID + "-FRIEND_REQUESTS_NOTIFICATIONS", MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.putString(senderID, notification.getUsername()).commit();
        }
        Log.d("SAVE_NOTIF", "guardando notificación -> " + notification.getMessage());
    }

    /** Remueve una notificación de un usuario en particular **/
    public static void removeNotificationsFromSender(int senderID, int notif_type, Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return;
        String myUserID = String.valueOf(credentials.getUserID());
        String senderID_s = String.valueOf(senderID);
        SharedPreferences sharedPref;
        if (notif_type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) {
            sharedPref = context.getSharedPreferences(myUserID + "-MESSAGE_NOTIFICATIONS", MODE_PRIVATE);
            SharedPreferences sharedPrefUsernames = context.getSharedPreferences(myUserID + "-USERNAMES_FROM_MESSAGE_NOTIFICATIONS", MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPref.edit();
            SharedPreferences.Editor editorUsernames = sharedPrefUsernames.edit();
            String messagesFromSender = sharedPref.getString(senderID_s, null);
            if (messagesFromSender != null) {
                editor.remove(senderID_s).commit();
                editorUsernames.remove(senderID_s).commit();
                editor.apply();
                editorUsernames.apply();
            }
        } else {
            sharedPref = context.getSharedPreferences(myUserID + "-FRIEND_REQUESTS_NOTIFICATIONS", MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.remove(senderID_s).commit();
            editor.apply();
        }
        Log.d("DELETE_NOTIF", "borrando notificación de: " + senderID_s);
    }

    public static ArrayList<AppServerNotification> loadPreviousMessagesNotifications(Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return null;
        String myUserID = String.valueOf(credentials.getUserID());
        ArrayList<AppServerNotification> notifications = new ArrayList<>();

        SharedPreferences sharedPrefMsgNotif = context.getSharedPreferences(myUserID + "-MESSAGE_NOTIFICATIONS", MODE_PRIVATE);
        SharedPreferences sharedPrefUsernames = context.getSharedPreferences(myUserID + "-USERNAMES_FROM_MESSAGE_NOTIFICATIONS", MODE_PRIVATE);

        Map<String, ?> senderIDs = sharedPrefMsgNotif.getAll();
        for (Map.Entry<String,?> entry : senderIDs.entrySet()) {
            int senderID = Integer.valueOf(entry.getKey());
            String username = sharedPrefUsernames.getString(entry.getKey().toString(), "unknown");
            String unreadMessages = entry.getValue().toString();
            AppServerNotification newNotif = new AppServerNotification(senderID, username, "", Constants.NOTIFICATION_TYPE_NEW_MESSAGE);

            ArrayList<String> messages = new ArrayList<String>(Arrays.asList(unreadMessages.split(";")));
            for (String msg : messages)
                newNotif.loadPreviousMessage(msg);

            notifications.add(newNotif);
        }
        if (notifications.isEmpty())
            return null;

        return notifications;
    }

    public static ArrayList<AppServerNotification> loadPreviousFriendRequestsNotifications(Context context) {
        UserCredentials credentials = loadUserCredentials(context);
        if (credentials == null)
            return null;
        String myUserID = String.valueOf(credentials.getUserID());
        ArrayList<AppServerNotification> notifications = new ArrayList<>();
        SharedPreferences sharedPref = context.getSharedPreferences(myUserID + "-FRIEND_REQUESTS_NOTIFICATIONS", MODE_PRIVATE);
        Map<String, ?> senderIDs = sharedPref.getAll();
        for (Map.Entry<String,?> entry : senderIDs.entrySet()) {
            int senderID = Integer.valueOf(entry.getKey());
            String name = entry.getValue().toString();
            notifications.add(new AppServerNotification(senderID, name, "", Constants.NOTIFICATION_TYPE_FRIEND_REQUEST));
        }
        if (notifications.isEmpty())
            return null;
        return notifications;
    }

}