package com.example.android.clientapp;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import java.util.ArrayList;

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

    public static String getTopTenPopURL() {
        return APPSERVER_URL+ USERS + POPULAR;
    }

    public static String getTopTenPopSkillURL(String skill) {
        return getTopTenPopURL()+"/skill/"+skill;
    }

    public static String getTopTenPopPuestoURL(String puesto) {
        return getTopTenPopURL()+"/position/"+puesto;
    }

    public static String getAdvBuscquedaURL(ArrayList<String> skills, ArrayList<String> puestos,
                                            double origenLongitud, double origenLatitud,
                                            double maxDist, boolean popSort) {
        //TODO: Devolver el resultado correcto
        String result = APPSERVER_URL+USERS+"/?";
        return result;
    }

    public static void setIP(String IP) {
        APPSERVER_IP = IP;
        APPSERVER_URL = "http://" + APPSERVER_IP + ":" + APPSERVER_PUERTO;
    }

    public static String getIP() { return APPSERVER_IP; }

}