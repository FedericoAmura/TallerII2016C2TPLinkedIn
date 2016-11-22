package com.example.android.clientapp.FCM;

import android.content.SharedPreferences;
import android.util.Log;

import com.example.android.clientapp.utils.PreferenceHandler;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;

/**
 * Created by emanuel on 11/10/16.
 */

public class MyFirebaseInstanceIdService extends FirebaseInstanceIdService {
    private static final String REG_TOKEN_GCM = "FCM_TOKEN";

    @Override
    public void onTokenRefresh() {
        String token = FirebaseInstanceId.getInstance().getToken();
        Log.d(REG_TOKEN_GCM, token);
        PreferenceHandler.updateFCMToken(getApplicationContext(), token);
    }
}
