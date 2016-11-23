package com.example.android.clientapp;

import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.ArrayAdapters.ChatRVAdapter;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.utils.Constants;
import com.example.android.clientapp.utils.NotificationEvent;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;

public class ChatListActivity extends AppCompatActivity {
    private EventBus bus = EventBus.getDefault();

    private RecyclerView rv;
    private LinearLayoutManager llm;

    private int statusCode;

    private ChatRVAdapter chatArrayAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        chatArrayAdapter = new ChatRVAdapter(getApplicationContext());

        setContentView(R.layout.recycler_view);
        setToolbar();

        rv = (RecyclerView) findViewById(R.id.rv);

        llm = new LinearLayoutManager(this);
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        rv.setAdapter(chatArrayAdapter);
    }

    // Carga los chats guardados.
    private void updateChatList() {
        ArrayList<Chat> chats = PreferenceHandler.getSavedConversations(this);
        for (Chat chat : chats) {
            updateUserThumbnailFromServer(chat.getReceiverID());
            chatArrayAdapter.add(chat);
        }
    }

    @Override
    public void onResume() {
        updateChatList();
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStart() {
        super.onStart();
        bus.register(this);
    }

    // Permite recibir notificaciones mientras está corriendo en esta activity
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(NotificationEvent notificationEvent) {
        RemoteMessage remoteMessage = notificationEvent.getRemoteMessage();
        int type = Integer.valueOf(remoteMessage.getData().get("type_notif"));
        if (type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE || type == Constants.NOTIFICATION_TYPE_FRIEND_REQUEST) //NEW MESSAGE OR FRIEND REQUEST TODO
            NotificationLauncher.launch(this, remoteMessage);
    }

    @Override
    protected void onStop() {
        super.onStop();
        bus.unregister(this);
    }

    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {// Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setTitle("Chats");
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_amigos, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case android.R.id.home:
                super.onBackPressed();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }


    private void showSnackBar(String msg) {
        Snackbar
                .make(findViewById(R.id.coordinator), msg, Snackbar.LENGTH_LONG)
                .show();
    }

    private void updateUserThumbnailFromServer(final int userID) {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getThumbnailURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            try {
                                String thumb_encoded = response.getString("thumb");
                                PreferenceHandler.updateUserThumbnail(userID, thumb_encoded, getApplicationContext());
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                    }
                }) {
            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }
}