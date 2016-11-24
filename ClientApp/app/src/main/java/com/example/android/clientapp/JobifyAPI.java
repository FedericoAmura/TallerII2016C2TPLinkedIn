package com.example.android.clientapp;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import static com.facebook.FacebookSdk.getApplicationContext;

/**
 * Created by guidonegri on 12/10/16.
 */

public class JobifyAPI {

    private static String APPSERVER_IP = "192.168.0.103";
    private static String APPSERVER_PUERTO = "8888";
    private static String APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;

    // Endpoints:
    private static final String LOGIN = "/login";
    private static final String SIGNUP = "/signup";
    private static final String USERS = "/users";
    private static final String CONTACTS = "/contacts";
    private static final String BRIEF = "/brief";
    private static final String POPULAR = "/popular";
    private static final String NOTIF = "/notif";

    public static String getLoginURL(){
        return APPSERVER_URL + LOGIN;
    }

    public static String getRegistroURL(){
        return APPSERVER_URL + SIGNUP;
    }

    public static String getPerfilURL(String userID) { return APPSERVER_URL + USERS + "/" + userID; }

    public static String getContactosURL(String userID) { return APPSERVER_URL + USERS + "/" + userID + CONTACTS; }

    public static String getContactoBriefURL(String userID) { return APPSERVER_URL + USERS + "/" + userID + BRIEF; }

    public static String getRecomendarURL(String recommendedUserID, String recommenderUserID) {
        return APPSERVER_URL + USERS + POPULAR + "/" + recommendedUserID + "/" + recommenderUserID; }

    public static String getSendMessageURL(int senderID, int receiverID) {
        return APPSERVER_URL + "/chat/" + String.valueOf(senderID) + "/" + String.valueOf(receiverID);
    }

    public static String getNotificacionesURL(String userID) { return APPSERVER_URL + USERS + "/" + userID + NOTIF; }

    public static String getNotificacionURL(String userID, String senderUserID) {
        return APPSERVER_URL + USERS + "/" + userID + NOTIF + "/" + senderUserID; }

    public static void setIP(String IP) {
        APPSERVER_IP = IP;
        APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;
    }

    public static String getIP() { return APPSERVER_IP; }

}