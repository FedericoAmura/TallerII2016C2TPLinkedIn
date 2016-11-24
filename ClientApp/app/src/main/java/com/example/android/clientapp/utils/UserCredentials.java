package com.example.android.clientapp.utils;

/**
 * Created by emanuel on 11/12/16.
 */

public class UserCredentials {
    private String username;
    private String password;
    private int userID;
    private String token;

    public UserCredentials(String username, String password, int userID, String token) {
        this.username = username;
        this.password = password;
        this.userID = userID;
        this.token = token;
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public int getUserID() {
        return userID;
    }

    public String getToken() {
        return token;
    }
}
