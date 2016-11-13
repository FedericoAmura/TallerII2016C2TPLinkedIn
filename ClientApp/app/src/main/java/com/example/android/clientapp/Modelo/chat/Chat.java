package com.example.android.clientapp.Modelo.chat;

/**
 * Created by emanuel on 11/12/16.
 */

public class Chat{
    private int senderID;
    private String name;
    private String last_msg;
    private String hour;

    public Chat(int userID, String name, String last_msg, String hour){
        this.senderID = userID;
        this.name = name;
        this.last_msg = last_msg;
        this.hour = hour;
    }

    public int getSenderID() {
        return senderID;
    }

    public String getName() {
        return name;
    }

    public String getLastMessage() {
        return last_msg;
    }

    public String getHour() {
        return hour;
    }

}
