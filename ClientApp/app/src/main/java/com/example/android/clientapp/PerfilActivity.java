package com.example.android.clientapp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.preference.PreferenceManager;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
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
import com.example.android.clientapp.Modelo.Perfil;
import com.example.android.clientapp.utils.Constants;
import com.example.android.clientapp.utils.NotificationEvent;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

public class PerfilActivity extends AppCompatActivity {
    private EventBus bus = EventBus.getDefault();
    public Perfil perfil;

    //Info personal:
    private TextView tvNombre;
    private ImageView ivFoto;
    private TextView tvEdad;
    private TextView tvCorreo;
    private TextView tvFav;
    private TextView tvRelaciones;
    private TextView tvUbicacion;

    //Resumen:
    private TextView tvResumen;

    private int statusCode;
    private UserCredentials credentials;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil);

        credentials = PreferenceHandler.loadUserCredentials(this);

        cargarDatosDelServer(String.valueOf(credentials.getUserID()));

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

    private void cargarBarra(){
        setToolbar();

        CollapsingToolbarLayout collapser =
                (CollapsingToolbarLayout) findViewById(R.id.collapser);
        collapser.setTitle("Perfil");

        loadImageParallax();
    }

    private void setToolbar() {
        // Agregar Toolbar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }

    private void loadImageParallax() {
        ImageView image = (ImageView) findViewById(R.id.image_paralax);
        image.setImageBitmap(perfil.getFoto());
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_perfil, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        switch (id) {
            case R.id.opcionCerrarSesion:
                apretarCerrarSesion();
                return true;
            case R.id.opcionEditar:
                apretarBotonEditar();
                return true;
            case R.id.opcionNotificaciones:
                apretarBotonNotificaciones();
                return true;
            case android.R.id.home:
                super.onBackPressed();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
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
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(PerfilActivity.this, "Perfil no cargado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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

    public void cargarPerfil(Perfil perfil){
        cargarBarra();
        //ivFoto = (ImageView) findViewById(R.id.person_photo);
        //ivFoto.setImageBitmap(perfil.getFoto());

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


        // Tv dinamicos de Skills:
        LinearLayout llSkills = (LinearLayout) findViewById(R.id.llSkills);
        for (int i = 0; i < perfil.getSizeSkills(); i++ ) {
            TextView tvSkill = new TextView(this);
            tvSkill.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT));
            tvSkill.setText("+" + perfil.getSkills(i));
            tvSkill.setTextColor(Color.GRAY);
            llSkills.addView(tvSkill);
        }

        // Tv dinamicos de ExpLaboral:
        LinearLayout llExpLaboral = (LinearLayout) findViewById(R.id.llExpLaboral);
        for (int i = 0; i < perfil.getSizeExp(); i++ ) {
            //for (int j = 0; j < 3; j++) {
                TextView tvExpLaboral = new TextView(this);
                tvExpLaboral.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT));
              //  switch (j) {
              //      case (0):
                        tvExpLaboral.setText("+" + perfil.getExpLaboralNombre(i));
              //          break;
                 /*   case (1):
                        tvExpLaboral.setText("     Desde: " + perfil.getExpLaboralInicio(i));
                        break;
                    case (2):
                        tvExpLaboral.setText("     Hasta: " + perfil.getExpLaboralFin(i));
                        break;
                */
              //  }
                tvExpLaboral.setTextColor(Color.GRAY);
                llExpLaboral.addView(tvExpLaboral);
            //}
        }

        tvResumen = (TextView) findViewById(R.id.tvResumen);
        tvResumen.setText(perfil.getResumen());

        tvUbicacion = (TextView) findViewById(R.id.tvUbicacion);
        tvUbicacion.setText(perfil.getCiudad());

        //Prueba longitud y latitud:
        /*TextView tvUbicacion2 = (TextView) view.findViewById(R.id.tvUbicacion2);
        tvUbicacion2.setText(String.valueOf(perfil.getLongitud()));

        TextView tvUbicacion3 = (TextView) view.findViewById(R.id.tvUbicacion3);
        tvUbicacion3.setText(String.valueOf(perfil.getLatitud()));*/
    }

    private void apretarCerrarSesion() {
        final String token = credentials.getToken();

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.DELETE, JobifyAPI.getLoginURL(), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            PreferenceHandler.removeCredentials(getApplicationContext());
                            volverLogin();
                            Toast.makeText(PerfilActivity.this, "Sesion cerrada.",Toast.LENGTH_LONG).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(PerfilActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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

    private void volverLogin(){
        Intent intent = new Intent(this, LoginActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }

    //Funcion a llamar al clickear boton Editar Perfil.
    private void apretarBotonEditar(){
        Intent intent = new Intent(this, PerfilEditActivity.class);
        perfil.setStrFoto("");
        String prf = perfil.crearJson().toString();
        intent.putExtra("perfil", prf);
        startActivity(intent);
    }

    private void apretarBotonNotificaciones(){
        Intent intent = new Intent(this, NotificacionesActivity.class);
        startActivity(intent);
    }

}
