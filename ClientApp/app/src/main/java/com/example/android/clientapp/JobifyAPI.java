package com.example.android.clientapp;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import static com.facebook.FacebookSdk.getApplicationContext;

/**
 * Created by guidonegri on 12/10/16.
 */

public class JobifyAPI {

    private static final String APPSERVER_IP = "192.168.0.106";
    private static final String APPSERVER_PUERTO = "8888";
    private static final String APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;

    // Endpoints:
    private static final String LOGIN = "/login";
    private static final String SIGNUP = "/signup";
    //private static final String PERFIL_USER = "/users/%s";
    private static final String PERFIL_USER = "/users/";

    public static String getLoginURL(){
        return APPSERVER_URL + LOGIN;
    }

    public static String getRegistroURL(){
        return APPSERVER_URL + SIGNUP;
    }

    public static String getPerfilURL(String userID) { return APPSERVER_URL + PERFIL_USER + userID; }

}