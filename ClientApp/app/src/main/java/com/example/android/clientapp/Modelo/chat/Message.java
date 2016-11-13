package com.example.android.clientapp.Modelo.chat;

import android.util.Log;

import java.util.Calendar;

/**
 * Created by emanuel on 11/12/16.
 */

public class Message {
    private String message;
    private boolean is_mine;
    private String hour;

    public Message(String msg, boolean is_mine) {
        this.message = msg;
        this.is_mine = is_mine;
        Calendar calendar = Calendar.getInstance();
        String h = String.valueOf(calendar.get(Calendar.HOUR_OF_DAY));
        int min = calendar.get(calendar.MINUTE);
        String m = String.valueOf(min);
        if (min < 10)
            m = "0" + m;
        this.hour = h + ":" + m;
    }

    public static Message hidrate(String stream) {
        String[] data = stream.split("-");
        boolean mine = ("0".equals(data[0])) ? true : false;
        String msg = data[1];
        String hour = data[2];
        Message message = new Message(msg, mine);
        message.hour = hour;
        return message;
    }

    public String getMessage() {
        return message;
    }

    public boolean is_mine() {
        return this.is_mine;
    }

    public String getHour() {
        return hour;
    }

}
