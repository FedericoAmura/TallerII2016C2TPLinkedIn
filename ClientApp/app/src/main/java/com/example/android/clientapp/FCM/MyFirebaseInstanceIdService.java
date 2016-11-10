package com.example.android.clientapp.FCM;

import android.content.SharedPreferences;
import android.util.Log;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;

/**
 * Created by emanuel on 11/10/16.
 */

public class MyFirebaseInstanceIdService extends FirebaseInstanceIdService {
    private static final String FILE_TOKEN = "FCMtoken";
    private static final String REG_TOKEN = "REG_TOKEN";

    @Override
    public void onTokenRefresh() {
        String token = FirebaseInstanceId.getInstance().getToken();
        Log.d(REG_TOKEN, token);
        SharedPreferences prefs = getSharedPreferences(FILE_TOKEN, MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString("token", token);
        editor.commit();
    }
}
