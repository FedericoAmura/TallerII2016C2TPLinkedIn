package com.example.android.clientapp;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import static com.facebook.FacebookSdk.getApplicationContext;

/**
 * Created by guidonegri on 12/10/16.
 */

public class JobifyAPI {

    private static String APPSERVER_IP = "192.168.1.37";
    private static String APPSERVER_PUERTO = "8888";
    private static String APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;

    // Endpoints:
    private static final String LOGIN = "/login";
    private static final String SIGNUP = "/signup";
    private static final String USERS = "/users";
    private static final String CONTACTS = "/contacts";
    private static final String BRIEF = "/brief";
    private static final String POPULAR = "/popular";

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

    public static String getThumbnailURL(int userID) {
        return APPSERVER_URL + "/users/" + String.valueOf(userID) + "/thumb";
    }

    public static String getNewChatURL(int userID) {
        return APPSERVER_URL + "/chat/" + String.valueOf(userID) + "/new";
    }

    public static String getIDLastMessageURL(int senderID, int receiverID) {
        return APPSERVER_URL + "/chat/" + String.valueOf(senderID) + "/" + String.valueOf(receiverID) + "/last";
    }

    public static String getMessagesInRange(int senderID, int receiverID, int start, int end) {
        return APPSERVER_URL + "/chat/" + String.valueOf(senderID) +
                "/" + String.valueOf(receiverID) + "/?start=" + String.valueOf(start) +
                "&end=" + String.valueOf(end);
    }

    public static String getNewChatMessages(int userID) {
        return APPSERVER_URL + "/chat/" + String.valueOf(userID) + "/new";
    }

    public static String getBriefChatDataURL(int chatterUID, int userID) {
        return APPSERVER_URL + "/users/" + String.valueOf(userID) + "/brief/" + String.valueOf(chatterUID);
    }

    public static void setIP(String IP) {
        APPSERVER_IP = IP;
        APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;
    }

    public static String getIP() { return APPSERVER_IP; }

}