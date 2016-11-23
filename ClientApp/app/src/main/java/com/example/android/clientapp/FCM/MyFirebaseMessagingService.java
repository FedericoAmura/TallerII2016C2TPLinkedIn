package com.example.android.clientapp.FCM;


import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.support.v7.app.NotificationCompat;
import android.util.Log;

import com.example.android.clientapp.R;
import com.example.android.clientapp.utils.Constants;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.MainActivity;
import com.example.android.clientapp.utils.NotificationEvent;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.EventBus;

/**
 * Created by emanuel on 11/10/16.
 */

public class MyFirebaseMessagingService extends FirebaseMessagingService {
    private EventBus bus = EventBus.getDefault();

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        showNotification(remoteMessage);
    }

    private void showNotification(RemoteMessage remoteMessage) {
        Log.d("NOTIFICATION_FIREBASE:", remoteMessage.getData().toString());
        if (!remoteMessage.getData().containsKey("type_notif"))
            return;
        NotificationEvent notificationEvent = new NotificationEvent(remoteMessage);
        // Lo mandamos por el bus (cuando la app corre en foreground)
        bus.post(notificationEvent);

        int type_notif = Integer.valueOf(remoteMessage.getData().get("type_notif"));

        NotificationCompat.Builder builder = null;
        PendingIntent pendingIntent = null;

        if ( type_notif != Constants.NOTIFICATION_TYPE_NEW_MESSAGE ||
             type_notif != Constants.NOTIFICATION_TYPE_FRIEND_REQUEST)
            return;
        NotificationLauncher.launch(getApplicationContext(), remoteMessage);

        Intent i = new Intent(getApplicationContext(), MainActivity.class);

        i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);

        pendingIntent = pendingIntent.getActivity(this, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);
        builder = (NotificationCompat.Builder) new NotificationCompat.Builder(this)
                                                .setAutoCancel(true)
                                                .setSmallIcon(R.mipmap.ic_launcher)
                                                .setContentIntent(pendingIntent);
        // Esto salta cuando la app corre en background
        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        manager.notify(0, builder.build());
    }
}
