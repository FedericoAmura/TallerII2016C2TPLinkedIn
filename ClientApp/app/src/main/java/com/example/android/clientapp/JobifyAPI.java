package com.example.android.clientapp;

/**
 * Created by guidonegri on 12/10/16.
 */

public class JobifyAPI {

    private static final String APPSERVER_IP = "192.168.0.102";
    private static final String APPSERVER_PUERTO = "8888";
    private static final String APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;

    // Endpoints:
    private static final String LOGIN = "/login";


    public static String getLoginURL(){
        return APPSERVER_URL + LOGIN;
    }



}