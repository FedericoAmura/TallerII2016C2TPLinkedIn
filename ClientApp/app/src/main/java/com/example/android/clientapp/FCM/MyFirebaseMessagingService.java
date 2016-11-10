package com.example.android.clientapp.FCM;


import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.support.v7.app.NotificationCompat;

import com.example.android.clientapp.LoginActivity;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

/**
 * Created by emanuel on 11/10/16.
 */

public class MyFirebaseMessagingService extends FirebaseMessagingService {
    private static final int TYPE_NOTIF_NEW_MSG = 1;
    private static final int TYPE_NOTIF_CONT_REQ = 2;

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        showNotification(remoteMessage);
    }

    private void showNotification(RemoteMessage remoteMessage) {
        int type_notif = Integer.getInteger(remoteMessage.getData().get("type_notif"));
        boolean valid_notif = true;

        NotificationCompat.Builder builder = null;
        Intent i = null;
        PendingIntent pendingIntent = null;

        switch (type_notif) {
            case TYPE_NOTIF_NEW_MSG:
                i = new Intent(this, LoginActivity.class);
                break;
            case TYPE_NOTIF_CONT_REQ:
                i = new Intent(this, LoginActivity.class);
                break;
            default:
                valid_notif = false;
                break;
        }

        if (!valid_notif)
            return;

        pendingIntent = pendingIntent.getActivity(this, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);
        builder = (NotificationCompat.Builder) new NotificationCompat.Builder(this)
                                                .setAutoCancel(true)
                                                .setContentIntent(pendingIntent);

        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        manager.notify(0, builder.build());
    }
}
