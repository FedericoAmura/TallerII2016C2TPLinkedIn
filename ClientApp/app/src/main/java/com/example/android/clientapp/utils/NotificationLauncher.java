package com.example.android.clientapp.utils;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v7.app.NotificationCompat;

import com.example.android.clientapp.ChatActivity;
import com.example.android.clientapp.R;
import com.google.firebase.messaging.RemoteMessage;

/**
 * Created by emanuel on 11/18/16.
 */

public class NotificationLauncher {
    private static final String GROUP_MESSAGES = "MESSAGES";
    private static final String GROUP_FRIEND_REQUESTS = "FRIEND_REQUESTS";

    public static void launch(Context context, RemoteMessage remoteMessage) {
        int notification_id = Integer.valueOf(remoteMessage.getData().get("type_notif"));
        int userID = Integer.valueOf(remoteMessage.getData().get("senderID"));
        String username = remoteMessage.getData().get("name");
        String message = remoteMessage.getNotification().getBody();

        String group = (notification_id == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) ? GROUP_MESSAGES : GROUP_FRIEND_REQUESTS;
        String title = (notification_id == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) ? "Nuevos Mensajes" : "Solicitud de Contacto";
        String line = (notification_id == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) ? username + " " + message : username + " quiere unirte a sus contactos";

        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);

        Intent notification_intent = new Intent(context, ChatActivity.class); // HARDCODE TODO
        notification_intent.putExtra("name", username);
        notification_intent.putExtra("receiverID", userID);
        PendingIntent pendingIntent = PendingIntent.getActivity(context,0,notification_intent,0);

        builder.setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(title)
                .setStyle(new NotificationCompat.InboxStyle()
                    .addLine(line)
                    .setBigContentTitle(title))
                .setAutoCancel(true)
                .setGroup(group)
                .setGroupSummary(true)
                .setContentIntent(pendingIntent);

        notificationManager.notify(notification_id, builder.build());
    }
}
