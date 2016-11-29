package com.example.android.clientapp.utils;

import com.google.firebase.messaging.RemoteMessage;

import java.util.ArrayList;

/**
 * Created by emanuel on 11/24/16.
 */

public class AppServerNotification {
    private int senderID;
    private String name;
    private String message;
    private ArrayList<String> messages = new ArrayList<>();
    private int notif_type;

    public AppServerNotification(RemoteMessage remoteMessage) {
        notif_type = Integer.valueOf(remoteMessage.getData().get("notif_type"));
        senderID = Integer.valueOf(remoteMessage.getData().get("senderID"));
        name = remoteMessage.getData().get("name");
        message = remoteMessage.getData().get("message");
    }

    public AppServerNotification(int senderID, String name, String msg, int notif_type) {
        this.senderID = senderID;
        this.name = name;
        this.message = msg;
        this.notif_type = notif_type;
    }

    public int getType() {
        return notif_type;
    }

    public int getSenderID() {
        return senderID;
    }

    public String getUsername() {
        return name;
    }

    public String getMessage() {
        return message;
    }

    public void loadPreviousMessage(String message) {
        if (notif_type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE)
            messages.add(message);
    }

    public ArrayList<String> getMessages() {
        if (notif_type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE)
            return messages;
        return null;
    }


}
