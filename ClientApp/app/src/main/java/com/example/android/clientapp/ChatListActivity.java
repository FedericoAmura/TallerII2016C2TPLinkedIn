package com.example.android.clientapp;

import android.content.SharedPreferences;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
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
import com.example.android.clientapp.ArrayAdapters.ChatRVAdapter;
import com.example.android.clientapp.Modelo.Amigo;
import com.example.android.clientapp.Modelo.chat.Chat;
import com.example.android.clientapp.utils.PreferenceHandler;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ChatListActivity extends AppCompatActivity {

    private static final String CONTACTS = "contacts";

    private static final String USER_ID = "userID";
    private static final String TOKEN = "token";

    private ArrayList<Amigo> amigos;
    private ArrayList<String> amigosID;
    private String userID;

    private RecyclerView rv;
    private LinearLayoutManager llm;

    private ArrayList<String> chatIDs = new ArrayList<>();

    private int statusCode;

    private ChatRVAdapter chatArrayAdapter;
    private ListView listView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        chatArrayAdapter = new ChatRVAdapter();

        setContentView(R.layout.recycler_view);
        setToolbar();

        rv = (RecyclerView) findViewById(R.id.rv);

        llm = new LinearLayoutManager(this);
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        rv.setAdapter(chatArrayAdapter);

        updateChatList();
    }

    private void updateChatList() {
        ArrayList<Chat> chats = PreferenceHandler.getSavedConversations(this);
        for (Chat chat : chats)
            chatArrayAdapter.add(chat);
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


    private void inicializarData(){
        amigos = new ArrayList<Amigo>();

        SharedPreferences chatsPref = getApplicationContext().getSharedPreferences("chats_user_"+ userID, MODE_PRIVATE);
        int chatSize = chatsPref.getInt("chatSize", 0);
        String chatID;
        for (String amigoUserID : amigosID) {
            Log.d("TEST", "AmigoUserID vale: " + amigoUserID);
            chatID = chatsPref.getString("chatID_"+ amigoUserID, null);
            if (chatID != null) {
                cargarAmigosDelServer(amigoUserID, chatSize);
            }
        }
    }

    private void inicializarAdapter(){
        Log.d("TEST", "AmigosChat size: " +amigos.size());
        RVAdapter adapter = new RVAdapter(amigos, ChatActivity.class);
        rv.setAdapter(adapter);
    }

    private void inicializarAmigosID(JSONObject obj){
        try {
            amigosID = new ArrayList<String>();
            JSONArray jsonContacts = obj.getJSONArray(CONTACTS);
            for (int i = 0; i < jsonContacts.length(); i++) {
                amigosID.add(jsonContacts.getString(i));
                Log.d("TEST", "AmigosChat ID size: " +amigosID.size());
            }
            inicializarData();
        } catch(JSONException e) {e.printStackTrace();}
    }

    private void cargarAmigosDelServer(final String userID, final int chatSize){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getContactoBriefURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Amigo amigo = new Amigo();
                            amigo.cargarDatosBriefDesdeJSON(response); //TODO: cambiar por el nuevo chat brief.
                            amigo.setUserID(userID);
                            amigos.add(amigo);
                            if (amigos.size() == chatSize) { inicializarAdapter(); }
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(ChatListActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);

    }


    private void cargarAmigosIdDelServer(final String userID, final String token){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getContactosURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            inicializarAmigosID(response);
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(ChatListActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(ChatListActivity.this, "Usuario no autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null) {
                            Toast.makeText(ChatListActivity.this,"ESTOY ACA 2:" + netResp.statusCode ,Toast.LENGTH_LONG).show();
                        }
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }

            @Override
            public Map<String,String> getHeaders() throws AuthFailureError {
                HashMap<String, String> params = new HashMap<String,String>();
                params.put("Authorization", "token="+token);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }



}