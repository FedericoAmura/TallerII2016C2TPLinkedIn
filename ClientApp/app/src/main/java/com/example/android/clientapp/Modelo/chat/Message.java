package com.example.android.clientapp.Modelo.chat;

/**
 * Created by emanuel on 11/12/16.
 */

public class Message {
    private String message;
    private boolean is_mine;

    public Message(String msg, boolean is_mine) {
        this.message = msg;
        this.is_mine = is_mine;
    }

    public String getMessage() {
        return message;
    }

    public boolean is_mine() {
        return this.is_mine;
    }

}
