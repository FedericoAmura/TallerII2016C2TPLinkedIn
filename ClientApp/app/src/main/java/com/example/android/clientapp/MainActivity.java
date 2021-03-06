package com.example.android.clientapp;

import android.content.Intent;
import android.os.Bundle;

import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.ActivityHandler;
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.Modelo.Perfil;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.RequestQueueSingleton;
import com.example.android.clientapp.utils.UserCredentials;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    private final String LOG_TAG = "MAIN_ACTIVITY";
    private EventBus bus = EventBus.getDefault();
    private UserCredentials credentials;
    public Perfil perfil;
    boolean exit = false;

    //Info personal:
    private CardView cvPerfil;
    private TextView tvNombre;
    private ImageView ivFoto;
    private TextView tvEdad;
    private TextView tvCorreo;
    private TextView tvFav;
    private TextView tvRelaciones;

    //Botones:
    private FloatingActionButton botonAmigos;
    private FloatingActionButton botonBusqueda;
    private FloatingActionButton botonChat;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        credentials = PreferenceHandler.loadUserCredentials(this);
        if (credentials == null) {
            ActivityHandler.launchLoginActivity(getApplicationContext());
            finish();
            return;
        }

//        cargarDatosDelServer(String.valueOf(credentials.getUserID()));

        cvPerfil = (CardView) findViewById(R.id.cvInfo);
        cvPerfil.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                verPerfil();
            }
        });

        botonAmigos = (FloatingActionButton) findViewById(R.id.boton_amigos);
        botonAmigos.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                verAmigos();
            }

        });

        botonChat = (FloatingActionButton) findViewById(R.id.boton_chat);
        botonChat.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                verChats();
            }

        });

        botonBusqueda = (FloatingActionButton) findViewById(R.id.boton_busqueda);
        botonBusqueda.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                busqueda();
            }
        });
    }

    // Nos registramos en el bus de eventos (llegada de notificaciones)
    @Override
    protected void onStart() {
        super.onStart();
        if (credentials != null)
            cargarDatosDelServer(String.valueOf(credentials.getUserID()));
        bus.register(this);
    }

    // Permite recibir notificaciones mientras está corriendo en esta activity
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(AppServerNotification notification) {
        NotificationLauncher.launch(this, notification);
    }

    @Override
    protected void onStop() {
        bus.unregister(this);
        super.onStop();
        RequestQueueSingleton.getInstance(this).cancelPendingRequests(LOG_TAG);
    }

    private void verChats() {
        Intent intent = new Intent(this, ChatListActivity.class);
        startActivity(intent);
    }

    private void verPerfil(){
        Intent intent = new Intent(this, PerfilActivity.class);
        startActivity(intent);
    }

    private void busqueda(){
        Intent intent = new Intent(this, BusquedasActivity.class);
        startActivity(intent);
    }

    private void verAmigos(){
        Intent intent = new Intent(this, AmigosActivity.class);
        startActivity(intent);
    }

    private void cargarDatosDelServer(final String userID){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getPerfilURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            perfil = new Perfil();
                            perfil.cargarDesdeJSON(response);
                            cargarPerfil(perfil);
                            //Toast.makeText(getActivity(),"Perfil cargado.",Toast.LENGTH_LONG).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                       // if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                       //}
                        Toast.makeText(getApplicationContext(),"No hay conexión con el Servidor.",Toast.LENGTH_LONG).show();
                        ActivityHandler.launchLoginActivity(getApplicationContext());
                        finish();
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
        jsonRequest.setTag(LOG_TAG);
        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
//        RequestQueue requestQueue = Volley.newRequestQueue(this);
//        requestQueue.add(jsonRequest);
    }

    public void cargarPerfil(Perfil perfil){
        ivFoto = (ImageView) findViewById(R.id.person_photo);
        ivFoto.setImageBitmap(perfil.getFoto());

        tvNombre = (TextView) findViewById(R.id.person_name);
        tvNombre.setText(perfil.getNombre());

        tvFav = (TextView) findViewById(R.id.person_fav);
        tvFav.setText(perfil.getCantRecomendaciones());

        tvRelaciones = (TextView) findViewById(R.id.person_relaciones);
        tvRelaciones.setText(perfil.getCantContactos());

        tvEdad = (TextView) findViewById(R.id.person_age);
        tvEdad.setText(perfil.getNacimiento());

        tvCorreo = (TextView) findViewById(R.id.person_email);
        tvCorreo.setText(perfil.getCorreo());
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        switch (id) {
            case R.id.opcionCerrarSesion:
                apretarCerrarSesion();
                return true;
            case R.id.opcionNotificaciones:
                apretarBotonNotificaciones();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }



    @Override
    public void onBackPressed(){
        super.onBackPressed();
    }


    private void apretarCerrarSesion() {
        final String token = credentials.getToken();

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.DELETE, JobifyAPI.getLoginURL(), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            ActivityHandler.launchLoginActivity(getApplicationContext());
                            Toast.makeText(MainActivity.this, "Sesion cerrada.",Toast.LENGTH_LONG).show();
                            finish();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(MainActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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

    private void apretarBotonNotificaciones(){
        Intent intent = new Intent(this, NotificacionesActivity.class);
        startActivity(intent);
    }

}