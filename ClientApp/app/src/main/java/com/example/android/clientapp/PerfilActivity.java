package com.example.android.clientapp;

import android.content.Intent;
import android.content.SharedPreferences;
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

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

import static com.facebook.FacebookSdk.getApplicationContext;

public class PerfilActivity extends AppCompatActivity {

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

    //Botones:
    private Button botonCerrarSesion;
    private Button botonEditar;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil);

        botonEditar = (Button) findViewById(R.id.boton_editar_perfil);
        botonEditar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonEditar();
            }

        });

        botonCerrarSesion = (Button) findViewById(R.id.boton_cerrar_sesion);
        botonCerrarSesion.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarCerrarSesion();
            }

        });


        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        String userID = sharedPref.getString("userID", "");

        cargarDatosDelServer(userID);

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

    private void showSnackBar(String msg) {
        Snackbar
                .make(findViewById(R.id.coordinator), msg, Snackbar.LENGTH_LONG)
                .show();
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
            case R.id.opcionAjustes:
                showSnackBar("Ajustes");
                return true;
            case R.id.opcionNotificaciones:
                showSnackBar("Notificaciones");
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
            tvSkill.setText(perfil.getSkills(i));
            tvSkill.setTextColor(Color.GRAY);
            llSkills.addView(tvSkill);
        }

        // Tv dinamicos de ExpLaboral:
        LinearLayout llExpLaboral = (LinearLayout) findViewById(R.id.llExpLaboral);
        for (int i = 0; i < perfil.getSizeExp(); i++ ) {
            for (int j = 0; j < 3; j++) {
                TextView tvExpLaboral = new TextView(this);
                tvExpLaboral.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT));
                switch (j) {
                    case (0):
                        tvExpLaboral.setText("- Lugar: " + perfil.getExpLaboralNombre(i));
                        break;
                    case (1):
                        tvExpLaboral.setText("     Desde: " + perfil.getExpLaboralInicio(i));
                        break;
                    case (2):
                        tvExpLaboral.setText("     Hasta: " + perfil.getExpLaboralFin(i));
                        break;
                }
                tvExpLaboral.setTextColor(Color.GRAY);
                llExpLaboral.addView(tvExpLaboral);
            }
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
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        final String token = sharedPref.getString("token", "");

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.DELETE, JobifyAPI.getLoginURL(), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
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
        intent.putExtra("perfil", perfil.crearJson().toString());
        startActivity(intent);
    }

}
