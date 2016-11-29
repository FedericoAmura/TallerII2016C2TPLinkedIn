package com.example.android.clientapp;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
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
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.CircleBitmap;
import com.example.android.clientapp.utils.Constants;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.Modelo.chat.Message;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONArray;
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
    private Bitmap thumbnail = null;
    private UserCredentials credentials;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        buttonSend = (Button) findViewById(R.id.btn_send);
        editTextSend = (EditText) findViewById(R.id.edit_msg);
        msglist = (ListView) findViewById(R.id.msg_list);

        messageArrayAdapter = new MessageArrayAdapter(getApplicationContext(), R.layout.right_message);
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
        thumbnail = getIntent().getParcelableExtra("thumbnail");

        if (thumbnail == null)
            loadUserThumbnailFromServer();

        setToolbar();

        msglist.setTranscriptMode(AbsListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
        msglist.setAdapter(messageArrayAdapter);

        PreferenceHandler.removeNotificationsFromSender(amigoUserID, Constants.NOTIFICATION_TYPE_NEW_MESSAGE, this);

        updateConversation();
    }

    private void updateConversation() {
        ArrayList<Message> messages = PreferenceHandler.loadMessages(amigoUserID, this);
        for (Message msg : messages)
            messageArrayAdapter.add(msg);
        checkForNewUnreadMessages();        // Chequeamos si hay mensajes no leidos
    }

    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null)
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle(nombreAmigo);
        if (thumbnail != null) {
            BitmapDrawable iconThumb = new BitmapDrawable(getResources(), CircleBitmap.resize_thumbnail(thumbnail, 60, 60));
            //getSupportActionBar().setHomeAsUpIndicator(iconThumb);
            toolbar.setNavigationIcon(iconThumb);
        } else {
            Bitmap thumb_default = BitmapFactory.decodeResource(getResources(), R.drawable.user_default);
            Bitmap circle_thumb = CircleBitmap.resize_thumbnail(CircleBitmap.generate(thumb_default), 60, 60);
            BitmapDrawable iconThumb = new BitmapDrawable(getResources(), circle_thumb);
            toolbar.setNavigationIcon(iconThumb);
        }
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
    public void onEvent(AppServerNotification notification) {
        int type = notification.getType();
        int senderID = notification.getSenderID();
        if (type == Constants.NOTIFICATION_TYPE_NEW_MESSAGE && senderID == amigoUserID) {
            receiveMessage(notification.getMessage());
            PreferenceHandler.removeNotificationsFromSender(amigoUserID, type, this);
        } else
            NotificationLauncher.launch(this, notification);
    }

    // Agrega los mensajes recibidos. Guarda el mensaje.
    private void receiveMessage(String msg) {
        Message message = new Message(msg, false);
        messageArrayAdapter.add(message);
        messageArrayAdapter.notifyDataSetChanged();
        NotifyMessageSeen();
        PreferenceHandler.saveMessage(amigoUserID, message, this);
        PreferenceHandler.saveLastChatMessage(new Chat(amigoUserID, nombreAmigo, message.getMessage(), 0), this);
        Log.d("SAVE_MESSAGE", msg);
    }

    // Envía el mensaje al servidor y guarda el mensaje.
    private void sendMessage() {
        Message message = new Message(editTextSend.getText().toString(), true);
        messageArrayAdapter.add(message);
        PreferenceHandler.saveMessage(amigoUserID, message, this);
        PreferenceHandler.saveLastChatMessage(new Chat(amigoUserID, nombreAmigo, message.getMessage(), 0), this);
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

    /** Notificar mensajes vistos (clavar visto) **/
    private void NotifyMessageSeen() {
        JSONObject data = new JSONObject();
        try {
            data.put("targetID", amigoUserID);
            data.put("userID", credentials.getUserID());
        } catch (JSONException e) {
            e.printStackTrace();
        }
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST,
                JobifyAPI.getNewChatURL(credentials.getUserID()), data,
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
                                Toast.makeText(ChatActivity.this, "No pudo clavar visto." +
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

    /** Chequeo for mensajes no leídos **/
    private void checkForNewUnreadMessages() {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getIDLastMessageURL(credentials.getUserID(), amigoUserID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            int id_last_message = response.getInt("lastmsg");
                            int numberMessages = messageArrayAdapter.getCount();
                            if (numberMessages - 1 < id_last_message)
                                getUnreadMessages(numberMessages, id_last_message);
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
                                Toast.makeText(ChatActivity.this, "No pudo obtener el id del ultimo mensaje." +
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

    /** Obtener mensajes no leídos **/
    private void getUnreadMessages(int start, int end) {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getMessagesInRange(credentials.getUserID(), amigoUserID, start, end), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            JSONArray messages = response.getJSONArray("messages");
                            for (int i = 0; i < messages.length(); i++) {
                                JSONObject msg = messages.getJSONObject(i);
                                int senderID = msg.getInt("senderID");
                                String message = msg.getString("message");
                                if (senderID == amigoUserID)
                                    receiveMessage(message);
                                else
                                    messageArrayAdapter.add(new Message(message, true));
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
                                Toast.makeText(ChatActivity.this, "No pudo obtener los mensajes comprendidos en un rango." +
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

    /** Update thumbnail **/
    private void loadUserThumbnailFromServer() {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getThumbnailURL(amigoUserID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            String thumb_encoded = response.getString("thumb");
                            PreferenceHandler.updateUserThumbnail(amigoUserID, thumb_encoded, getApplicationContext());
                            thumbnail = PreferenceHandler.getUserThumbnail(amigoUserID, getApplicationContext());
                            setToolbar();
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
                                Toast.makeText(ChatActivity.this, "No pudo obtener el thumbnail del amigo." +
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
