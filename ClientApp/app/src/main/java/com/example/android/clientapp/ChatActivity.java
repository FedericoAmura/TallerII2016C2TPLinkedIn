package com.example.android.clientapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.AbsListView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.ArrayAdapters.MessageArrayAdapter;
import com.example.android.clientapp.utils.NotificationEvent;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.Modelo.chat.Message;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import org.greenrobot.eventbus.EventBus;

public class ChatActivity extends AppCompatActivity {
    private MessageArrayAdapter messageArrayAdapter;
    private ListView msglist;
    private Button buttonSend;
    private EditText editTextSend;
    private int amigoUserID;
    private String nombreAmigo = "?";
    private UserCredentials credentials;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        buttonSend = (Button) findViewById(R.id.btn_send);
        editTextSend = (EditText) findViewById(R.id.edit_msg);
        msglist = (ListView) findViewById(R.id.msg_list);

        messageArrayAdapter = new MessageArrayAdapter(getApplicationContext(), R.layout.right);
        msglist.setAdapter(messageArrayAdapter);

        buttonSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!editTextSend.getText().toString().isEmpty())
                    sendMessage();
            }
        });

        credentials = PreferenceHandler.loadUserCredentials(this);

        Bundle bundle = getIntent().getExtras();
        amigoUserID = bundle.getInt("receiverID");
        nombreAmigo = bundle.getString("name");

        setToolbar();

        msglist.setTranscriptMode(AbsListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
        msglist.setAdapter(messageArrayAdapter);

        // Tests
        messageArrayAdapter.add(new Message("Hola", false));
        messageArrayAdapter.add(new Message("Hola1", false));
        messageArrayAdapter.add(new Message("Hola2", false));
        messageArrayAdapter.add(new Message("Hola4", true));
        messageArrayAdapter.add(new Message("Hola", false));
        messageArrayAdapter.add(new Message("Hola1", true));
        messageArrayAdapter.add(new Message("Hola2", false));
        messageArrayAdapter.add(new Message("Hola4", true));

        updateConversations();
    }

    private void updateConversations() {
        ArrayList<Message> messages = PreferenceHandler.loadMessages(amigoUserID, this);
        for (Message msg : messages)
            messageArrayAdapter.add(msg);
    }

    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null)
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle(nombreAmigo);
    }

    @Override
    protected void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    protected void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    // Permite recibir notificaciones mientras está corriendo en esta activity
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(NotificationEvent notificationEvent) {
        RemoteMessage remoteMessage = notificationEvent.getRemoteMessage();
        int type = Integer.valueOf(remoteMessage.getData().get("type_notif"));
        int senderID = Integer.valueOf(remoteMessage.getData().get("senderID"));
        if (type == 1 && senderID == amigoUserID) {//Nuevo mensaje
            receiveMessage(remoteMessage.getNotification().getBody());
            // notificar mensaje visto (TODO)
        }
        else {
            NotificationLauncher.launch(getApplicationContext(), remoteMessage);
        }
    }

    // Agrega los mensajes recibidos. Guarda el mensaje.
    private void receiveMessage(String msg) {
        Message message = new Message(msg, false);
        messageArrayAdapter.add(message);
        messageArrayAdapter.notifyDataSetChanged();
        PreferenceHandler.saveMessage(amigoUserID, message, this);
        PreferenceHandler.saveLastChatMessage(new Chat(amigoUserID, nombreAmigo, message.getMessage()), this);
    }

    // Envía el mensaje al servidor y guarda el mensaje.
    private void sendMessage() {
        Message message = new Message(editTextSend.getText().toString(), true);
        messageArrayAdapter.add(message);
        PreferenceHandler.saveMessage(amigoUserID, message, this);
        PreferenceHandler.saveLastChatMessage(new Chat(amigoUserID, nombreAmigo, message.getMessage()), this);
        sendMessageToServer(message);
        editTextSend.setText("");
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }

    private void sendMessageToServer(Message message) {
        JSONObject data = new JSONObject();
        try {
            data.put("receiverID", amigoUserID);
            data.put("senderID", credentials.getUserID());
            data.put("message", message.getMessage());
        } catch (JSONException e) {
            e.printStackTrace();
        }
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST,
                                JobifyAPI.getSendMessageURL(credentials.getUserID(), amigoUserID), data,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
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
                                    Toast.makeText(ChatActivity.this, "No pudo enviarse el mensaje." +
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
