package com.example.android.clientapp.FCM;


import android.app.Notification;
import android.support.v4.app.NotificationManagerCompat;
import android.util.Log;

import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.DataBaseNotifHelper;
import com.example.android.clientapp.utils.PreferenceHandler;
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
        Log.d("FIREBASE_NOTIFICATION:", remoteMessage.getData().toString());
        if (!remoteMessage.getData().containsKey("notif_type"))
            return;

        AppServerNotification appServerNotification = new AppServerNotification(remoteMessage);
        PreferenceHandler.saveNotification(appServerNotification, getApplicationContext());

        if (bus.hasSubscriberForEvent(AppServerNotification.class)) {
            // Lo mandamos por el bus (cuando la app corre en foreground)
            bus.post(appServerNotification);
            return;
        }

        // Esto salta cuando la app corre en background
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        Notification notification = DataBaseNotifHelper.generateNotification(appServerNotification, this);
        if (notification != null)
            notificationManager.notify(appServerNotification.getType(), notification);

    }
}
