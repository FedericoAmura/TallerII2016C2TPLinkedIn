package com.example.android.clientapp.utils;

import android.app.Notification;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.NotificationCompat;

import com.example.android.clientapp.ChatActivity;
import com.example.android.clientapp.ChatListActivity;
import com.example.android.clientapp.MainActivity;
import com.example.android.clientapp.R;

import java.util.ArrayList;

/**
 * Created by emanuel on 11/24/16.
 */

public class DataBaseNotifHelper {

    public static Notification generateNotification(AppServerNotification newAppServerNotification, Context context) {

        ArrayList<AppServerNotification> notifications = null;
        int type = newAppServerNotification.getType();

        switch (type) {
            case Constants.NOTIFICATION_TYPE_NEW_MESSAGE:
                notifications = PreferenceHandler.loadPreviousMessagesNotifications(context);
                break;
            case  Constants.NOTIFICATION_TYPE_FRIEND_REQUEST:
                notifications = PreferenceHandler.loadPreviousFriendRequestsNotifications(context);
                break;
            default:
                break;
        }

        if (notifications == null)
            return null;

        if (notifications.size() == 1)
            return generateNotificationFromSender(notifications.get(0), context);
        else
            return generateNotificationFromMultipleSenders(notifications, context);
    }

    private static Notification generateNotificationFromSender(AppServerNotification appServerNotification, Context context) {
        Intent intent;
        PendingIntent pendingIntent;
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);

        if (appServerNotification.getType() == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) {
            intent = new Intent(context, ChatActivity.class);
       //     Bitmap thumb = PreferenceHandler.getUserThumbnail(notification.getSenderID(), context);
       //     Bitmap circle_thumb = CircleBitmap.generate(thumb);

            intent.putExtra("name", appServerNotification.getUsername());
            intent.putExtra("receiverID", appServerNotification.getSenderID());
        //    intent.putExtra("thumbnail", CircleBitmap.resize_thumbnail(circle_thumb, 60, 60));

            pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);

            builder.setContentTitle("Nuevos Mensajes")
                    .setContentText("Tenés " + String.valueOf(appServerNotification.getMessages().size()) + " mensajes nuevos")
                    .setSmallIcon(R.drawable.ic_chat)
                    .setAutoCancel(true)
                    .setContentIntent(pendingIntent)
                    .setPriority(Notification.PRIORITY_MAX);

            NotificationCompat.InboxStyle not_inbox_style = new NotificationCompat.InboxStyle(builder);
            not_inbox_style.setBigContentTitle(appServerNotification.getUsername());
            ArrayList<String> messages = appServerNotification.getMessages();

            int top = 5;
            if (messages.size() < top)
                for (String msg : messages)
                    not_inbox_style.addLine(msg);
             else {
                for (int i = 0; i < top; i++)
                    not_inbox_style.addLine(messages.get(i));
                if (top != messages.size())
                    not_inbox_style.setSummaryText("+" + String.valueOf(messages.size()-top) + " mensajes");
            }

            builder.setStyle(not_inbox_style);

        } else {
            // TODO hardcode, debería ir a una pantalla de aceptar/rechazar solicitud
            intent = new Intent(context, MainActivity.class);

            pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);
            builder.setContentTitle("Solicitud de Contacto")
                    .setContentText("Tenés 1 solicitud de contacto")
                    .setSmallIcon(R.drawable.ic_friends)
                    .setAutoCancel(true)
                    .setContentIntent(pendingIntent)
                    .setPriority(Notification.PRIORITY_MAX);

            NotificationCompat.InboxStyle not_inbox_style = new NotificationCompat.InboxStyle(builder);
            not_inbox_style.setBigContentTitle("Solicitud de Contacto");
            not_inbox_style.addLine(appServerNotification.getUsername() + " quiere agregarte");

            builder.setStyle(not_inbox_style);
        }
        return builder.build();
    }

    private static Notification generateNotificationFromMultipleSenders(ArrayList<AppServerNotification> notifications, Context context) {
        Intent intent;
        PendingIntent pendingIntent;
        NotificationCompat.Builder builder;
        int count = notifications.size();

        if (notifications.get(0).getType() == Constants.NOTIFICATION_TYPE_NEW_MESSAGE) {
            intent = new Intent(context, ChatListActivity.class);
            pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);

            builder = (NotificationCompat.Builder) new NotificationCompat.Builder(context)
                    .setContentTitle("Nuevos Mensajes")
                    .setContentText("Tenés " + String.valueOf(count) + " nuevos chats")
                    .setSmallIcon(R.drawable.ic_chat)
                    .setAutoCancel(true)
                    .setContentIntent(pendingIntent)
                    .setPriority(Notification.PRIORITY_MAX);

            NotificationCompat.InboxStyle not_inbox_style = new NotificationCompat.InboxStyle(builder);
            not_inbox_style.setBigContentTitle(String.valueOf(count) + " nuevos chats");

            int top = 5;
            if (notifications.size() < top)
                for (AppServerNotification notification : notifications) {
                    String first_msg = notification.getMessages().get(0);
                    if (first_msg.length() > 20)
                        not_inbox_style.addLine(notification.getUsername() + ": " + first_msg.substring(0,20) + "...");
                    else
                        not_inbox_style.addLine(notification.getUsername() + ": " + first_msg);
                }
            else {
                for (int i = 0; i < top; i++) {
                    AppServerNotification notification = notifications.get(i);
                    String first_msg = notification.getMessages().get(0);
                    if (first_msg.length() > 10)
                        not_inbox_style.addLine(notification.getUsername() + ": " + first_msg.substring(0, 10) + "...");
                    else
                        not_inbox_style.addLine(notification.getUsername() + ": " + first_msg);
                    if (top != notifications.size())
                        not_inbox_style.setSummaryText("+" + String.valueOf(notifications.size()-top) + " chats");
                }
            }
            builder.setStyle(not_inbox_style);
        } else {
            // TODO hardcode, debería ir a una pantalla de aceptar/rechazar solicitudes
            intent = new Intent(context, MainActivity.class);

            pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);

            builder = (NotificationCompat.Builder) new NotificationCompat.Builder(context)
                    .setContentTitle("Solicitudes de Contacto")
                    .setContentText("Tenés " + String.valueOf(count) + " nuevas solicitudes")
                    .setSmallIcon(R.drawable.ic_friends)
                    .setAutoCancel(true)
                    .setContentIntent(pendingIntent)
                    .setPriority(Notification.PRIORITY_MAX);

            NotificationCompat.InboxStyle not_inbox_style = new NotificationCompat.InboxStyle(builder);
            not_inbox_style.setBigContentTitle(String.valueOf(count) + " nuevas solicitudes");

            int top = 5;
            if (notifications.size() < top)
                for (AppServerNotification notification : notifications)
                    not_inbox_style.addLine(notification.getUsername() + " quiere agregarte");
            else {
                for (int i = 0; i < top; i++) {
                    AppServerNotification notification = notifications.get(i);
                    not_inbox_style.addLine(notification.getUsername() + " quiere agregarte");
                    if (top != notifications.size())
                        not_inbox_style.setSummaryText("+" + String.valueOf(notifications.size()-top) + " solicitudes");
                }
            }
            builder.setStyle(not_inbox_style);
        }
        return builder.build();
    }
}
