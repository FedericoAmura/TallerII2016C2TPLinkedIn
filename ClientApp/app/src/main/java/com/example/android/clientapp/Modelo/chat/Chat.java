package com.example.android.clientapp.Modelo.chat;

import java.util.Calendar;

/**
 * Created by emanuel on 11/12/16.
 */

public class Chat{
    private int receiverID;
    private String name;
    private String last_msg;
    private String hour;

    public Chat(int userID, String name, String last_msg){
        this.receiverID = userID;
        this.name = name;
        this.last_msg = last_msg;
        Calendar calendar = Calendar.getInstance();
        String h = String.valueOf(calendar.get(Calendar.HOUR_OF_DAY));
        int min = calendar.get(calendar.MINUTE);
        String m = String.valueOf(min);
        if (min < 10)
            m = "0" + m;
        this.hour = h + ":" + m;
    }

    public static Chat hydrate(int userID, String stream) {
        String[] data = stream.split(";");
        String name = data[0];
        String last_msg = data[1];
        String hour = data[2];
        Chat chat = new Chat(userID, name, last_msg);
        chat.hour = hour;
        return chat;
    }

    public int getReceiverID() {
        return receiverID;
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

    @Override
    public boolean equals(Object o) {
        if (o == null) return false;
        Chat chat = (Chat) o;
        return this.receiverID == chat.receiverID;
    }
}
