package com.example.android.clientapp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.preference.PreferenceManager;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
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
import com.example.android.clientapp.utils.ActivityHandler;
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.CircleBitmap;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.RequestQueueSingleton;
import com.example.android.clientapp.utils.UserCredentials;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

public class PerfilAmigoActivity extends AppCompatActivity {
    private final String LOG_TAG = "PERFILAMIGO_ACTIVITY";
    private EventBus bus = EventBus.getDefault();

    private Perfil perfil;
    private String amigoUserID;

    private boolean recomendado = false;

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil_amigo);

        Intent i = getIntent();
        amigoUserID = (String) i.getExtras().get("userID");

        cargarDatosDelServer(amigoUserID);
    }

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


    private void cargarPantalla(){
        setToolbar();

        CollapsingToolbarLayout collapser =
                (CollapsingToolbarLayout) findViewById(R.id.collapser);
        collapser.setTitle(perfil.getNombre());

        loadImageParallax();

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        recomendarUsuario();
                    }
                }
        );
    }

    private void setToolbar() {
        // Añadir la Toolbar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        if (getSupportActionBar() != null)
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }

    private void loadImageParallax() {
        ImageView image = (ImageView) findViewById(R.id.image_paralax);
        image.setImageBitmap(perfil.getFoto());
    }

    private void showSnackBar(String msg) {
        Snackbar
                .make(findViewById(R.id.coordinator), msg, Snackbar.LENGTH_LONG)
                .show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_perfil_amigo, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        switch (id) {
            case R.id.opcionAgregar:
                agregarUsuario();
                return true;
            case R.id.opcionChat:
                abrirChat();
                return true;
            case android.R.id.home:
                super.onBackPressed();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }


    }

    private void abrirChat(){
        Intent intent = new Intent(this, ChatActivity.class);
        intent.putExtra("receiverID", Integer.valueOf(amigoUserID));
        intent.putExtra("name", tvNombre.getText().toString());
        Bitmap thumb = CircleBitmap.generate(perfil.getFoto());
        intent.putExtra("thumbnail", CircleBitmap.resize_thumbnail(thumb, 60, 60));
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
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(PerfilAmigoActivity.this, "Perfil no cargado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
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
        cargarPantalla();
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
            tvSkill.setText(perfil.getSkills(i));
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

    private void recomendarUsuario(){
        final UserCredentials credentials = PreferenceHandler.loadUserCredentials(getApplicationContext());

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.putOpt("recommender", credentials.getUserID());
            jsonObj.putOpt("recommended", Integer.valueOf(amigoUserID));
            if (! recomendado) {
                jsonObj.putOpt("recommends", true);
                recomendado = true;
            } else {
                jsonObj.putOpt("recommends", false);
                recomendado = false;
            }
        } catch (JSONException e) { }

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.PUT, JobifyAPI.getRecomendarURL(amigoUserID, String.valueOf(credentials.getUserID())), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(PerfilAmigoActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(PerfilAmigoActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            ActivityHandler.launchLoginActivity(getApplicationContext());
                            PreferenceHandler.removeCredentials(getApplicationContext());
                            finish();
                        }
                        else {
                            int rec = Integer.parseInt(perfil.getCantRecomendaciones());
                            if (recomendado) {
                                rec = rec + 1;
                                tvFav.setText(String.valueOf(rec));
                            }
                            else {
                                //rec = rec;
                                tvFav.setText(String.valueOf(rec));}
                        }
                    }
                }) {

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


    private void agregarUsuario(){
        final UserCredentials credentials = PreferenceHandler.loadUserCredentials(getApplicationContext());

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.putOpt("userID", credentials.getUserID());
            jsonObj.putOpt("targetID", Integer.valueOf(amigoUserID));
            jsonObj.putOpt("message", " ");
        } catch (JSONException e) { }

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST, JobifyAPI.getContactosURL(String.valueOf(credentials.getUserID())), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK) {
                            Toast.makeText(PerfilAmigoActivity.this, "Solicitud de amistad enviada.", Toast.LENGTH_LONG).show();
                        }
                        else { Toast.makeText(PerfilAmigoActivity.this, "Solicitud de amistad enviada. 2222222", Toast.LENGTH_LONG).show(); }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(PerfilAmigoActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(PerfilAmigoActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            ActivityHandler.launchLoginActivity(getApplicationContext());
                            PreferenceHandler.removeCredentials(getApplicationContext());
                            finish();
                        }
                        else {
                            Toast.makeText(PerfilAmigoActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                    }
                }) {

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
}
