package com.example.android.clientapp.utils;

import android.content.Context;
import android.content.Intent;

import com.example.android.clientapp.LoginActivity;

/**
 * Created by emanuel on 12/15/16.
 */

public class ActivityHandler {

    public static void launchLoginActivity(Context context) {
        Intent intent = new Intent(context, LoginActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }
}
