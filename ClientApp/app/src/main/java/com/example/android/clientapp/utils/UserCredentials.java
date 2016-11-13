package com.example.android.clientapp.utils;

/**
 * Created by emanuel on 11/12/16.
 */

public class UserCredentials {
    private int userID;
    private String token;

    public UserCredentials(int userID, String token) {
        this.userID = userID;
        this.token = token;
    }

    public int getUserID() {
        return userID;
    }

    public String getToken() {
        return token;
    }
}
