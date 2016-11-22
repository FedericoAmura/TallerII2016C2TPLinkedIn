package com.example.android.clientapp.utils;

import com.google.firebase.messaging.RemoteMessage;

/**
 * Created by emanuel on 11/17/16.
 */

public class NotificationEvent {
    RemoteMessage remoteMessage;

    public NotificationEvent(RemoteMessage remoteMessage) {
        this.remoteMessage = remoteMessage;
    }

    public RemoteMessage getRemoteMessage() {
        return remoteMessage;
    }
}
