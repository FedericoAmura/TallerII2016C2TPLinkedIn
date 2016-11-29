package com.example.android.clientapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.NotificationLauncher;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

/**
 * Clase de la cual heredar para implementar activities que reciben notificaciones
 */
public class NotifiableActivity extends AppCompatActivity {

    private EventBus bus = EventBus.getDefault();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    /**
     * Nos registramos en el bus de eventos (llegada de notificaciones)
     */
    @Override
    protected void onStart() {
        super.onStart();
        bus.register(this);
    }

    /**
     * Permite recibir notificaciones mientras está corriendo en esta activity
     */
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(AppServerNotification notification) {
        NotificationLauncher.launch(this, notification);
    }

    @Override
    protected void onStop() {
        super.onStop();
        bus.unregister(this);
    }
}
