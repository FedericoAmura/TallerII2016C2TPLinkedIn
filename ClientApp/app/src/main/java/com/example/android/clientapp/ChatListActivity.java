package com.example.android.clientapp;

import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.ArrayAdapters.ChatRVAdapter;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.Constants;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ChatListActivity extends AppCompatActivity {
    private EventBus bus = EventBus.getDefault();
    private UserCredentials credentials;

    private RecyclerView rv;
    private LinearLayoutManager llm;

    private int statusCode;

    private ChatRVAdapter chatArrayAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        credentials = PreferenceHandler.loadUserCredentials(this);

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
        checkForNewUnreadMessages();
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
    public void onEvent(AppServerNotification notification) {
        int type = notification.getType();
        if (type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE)
            checkForNewUnreadMessages();
        else
            NotificationLauncher.launch(this, notification);
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

    /** Actualización de thumbnails de los usuarios en caso de que cambien su foto durante el chat **/
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

    /** Chequea si hay chat pendientes/nuevos **/
    private void checkForNewUnreadMessages() {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getNewChatMessages(credentials.getUserID()), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            JSONArray new_chats = response.getJSONArray("new");
                            for (int i = 0; i < new_chats.length(); i++) {
                                JSONObject chat = new_chats.getJSONObject(i);
                                int senderID = chat.getInt("senderID");
                                int count_msg = chat.getInt("count");
                                addNewUnreadChat(senderID, count_msg);
                            }
                        } catch (JSONException e) {
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null)
                            if (netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND ||
                                    netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN ||
                                    netResp.statusCode == HttpURLConnection.HTTP_NOT_ACCEPTABLE ) {
                                Toast.makeText(ChatListActivity.this, "No pudo obtener los nuevos chats." +
                                        " CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                    }
                }){

            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                HashMap<String, String> params = new HashMap<String,String>();
                String token = credentials.getToken();
                params.put("Authorization", "token="+token);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }

    /** Agrega el nuevo chat a ChatList **/
    private void addNewUnreadChat(final int senderID, final int count_unread_msg) {

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getBriefChatDataURL(credentials.getUserID(), senderID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                                String name = response.getString("name");
                                String thumb = response.getString("thumb");
                                String last_message = response.getString("msg");
                                Chat newUnreadChat = new Chat(senderID, name, last_message, count_unread_msg);
                                chatArrayAdapter.add(newUnreadChat);
                                PreferenceHandler.updateUserThumbnail(senderID, thumb, getApplicationContext());
                            } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null)
                            if (netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND ||
                                    netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN ||
                                    netResp.statusCode == HttpURLConnection.HTTP_NOT_ACCEPTABLE ) {
                                Toast.makeText(ChatListActivity.this, "No pudo obtener los datos breves de chat con el usuario." +
                                        " CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                    }
                }){

            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                HashMap<String, String> params = new HashMap<String,String>();
                String token = credentials.getToken();
                params.put("Authorization", "token="+token);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }
}