package com.example.android.clientapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
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
import com.example.android.clientapp.Modelo.Amigo;
import com.example.android.clientapp.utils.ActivityHandler;
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.RequestQueueSingleton;
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

public class NotificacionesActivity extends AppCompatActivity {
    private final String LOG_TAG = "NOTIFICACIONES_ACTIVITY";
    private EventBus bus = EventBus.getDefault();
    private static final String PENDING = "pending";

    private ArrayList<Amigo> amigos;
    private ArrayList<String> notifID;

    private RecyclerView rv;
    private LinearLayoutManager llm;

    private int statusCode;
    private UserCredentials credentials;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recycler_view);

        rv = (RecyclerView) findViewById(R.id.rv);

        llm = new LinearLayoutManager(this);
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        credentials = PreferenceHandler.loadUserCredentials(this);

        cargarNotificacionesIdDelServer(String.valueOf(credentials.getUserID()), credentials.getToken());
        setToolbar();
    }


    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle("Notificaciones");
    }

    //Nos registramos en el bus de eventos (llegada de notificaciones)
    @Override
    protected void onStart() {
        super.onStart();
        bus.register(this);
    }

    // Permite recibir notificaciones mientras está corriendo en esta activity
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(AppServerNotification notification) {
        NotificationLauncher.launch(this, notification);
    }

    @Override
    protected void onStop() {
        super.onStop();
        bus.unregister(this);
        RequestQueueSingleton.getInstance(this).cancelPendingRequests(LOG_TAG);
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
                Intent intent = new Intent(this, MainActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void inicializarData(){
        amigos = new ArrayList<Amigo>();
        for (String senderUserID : notifID) {
            cargarNotificacionesDelServer(senderUserID);
        }
    }

    private void inicializarAdapter(){
        Log.d("TEST", "Anigos notif size: " +amigos.size());
        NotifAdapter adapter = new NotifAdapter(amigos);
        rv.setAdapter(adapter);
    }

    private void inicializarNotificacionesID(JSONObject obj){
        try {
            notifID = new ArrayList<String>();
            JSONArray jsonContacts = obj.getJSONArray(PENDING);
            for (int i = 0; i < jsonContacts.length(); i++) {
                notifID.add(jsonContacts.getString(i));
                Log.d("TEST", "Notif ID size: " + notifID.size());
            }
            inicializarData();
        } catch(JSONException e) {e.printStackTrace();}
    }

    private void cargarNotificacionesDelServer(final String senderUserID){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getContactoBriefURL(senderUserID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Amigo amigo = new Amigo();
                            amigo.cargarDatosBriefDesdeJSON(response);
                            amigo.setUserID(senderUserID);
                            amigos.add(amigo);
                            try {
                                PreferenceHandler.updateUserThumbnail(Integer.valueOf(senderUserID), response.getString("thumb"), getApplicationContext());
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                            if (amigos.size() == notifID.size()) { inicializarAdapter(); }
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(NotificacionesActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }

                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(NotificacionesActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            ActivityHandler.launchLoginActivity(getApplicationContext());
                            PreferenceHandler.removeCredentials(getApplicationContext());
                            finish();
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
                params.put("Authorization", "token="+credentials.getToken());
                return params;
            }
        };
        jsonRequest.setTag(LOG_TAG);
        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
//        RequestQueue requestQueue = Volley.newRequestQueue(this);
//        requestQueue.add(jsonRequest);

    }

    private void cargarNotificacionesIdDelServer(final String userID, final String token){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getNotificacionesURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Log.d("TEST", "Cargar Notif ID del server response OK");
                            inicializarNotificacionesID(response);
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(NotificacionesActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(NotificacionesActivity.this, "Usuario no autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            ActivityHandler.launchLoginActivity(getApplicationContext());
                            PreferenceHandler.removeCredentials(getApplicationContext());
                            finish();
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
        jsonRequest.setTag(LOG_TAG);
        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
//        RequestQueue requestQueue = Volley.newRequestQueue(this);
//        requestQueue.add(jsonRequest);
    }

}