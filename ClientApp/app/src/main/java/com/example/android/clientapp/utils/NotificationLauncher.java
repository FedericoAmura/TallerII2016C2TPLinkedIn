package com.example.android.clientapp.utils;

import android.app.Notification;
import android.content.Context;
import android.support.v4.app.NotificationManagerCompat;

/**
 * Created by emanuel on 11/18/16.
 */

public class NotificationLauncher {


    public static void launch(Context context, AppServerNotification appServerNotification) {
        int notif_type = appServerNotification.getType();
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
        Notification notification = DataBaseNotifHelper.generateNotification(appServerNotification, context);
        if (notification != null) {
            notificationManager.cancel(notif_type);
            notificationManager.notify(notif_type, notification);
        }
    }
}
