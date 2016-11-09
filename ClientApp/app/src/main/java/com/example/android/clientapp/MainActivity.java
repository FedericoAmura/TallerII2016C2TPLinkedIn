package com.example.android.clientapp;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;

import android.os.Handler;
import android.preference.PreferenceManager;
import android.support.annotation.BoolRes;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.Modelo.Perfil;

import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
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

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        String userID = sharedPref.getString("userID", "");

        cargarDatosDelServer(userID);

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
        /*botonChat.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){

            }

        });*/
    }

    private void apretarBotonChat(View view) {
        //Intent intent = new Intent(this, ChatActivity.class);
        //startActivity(intent);
    }

    private void verPerfil(){
        Intent intent = new Intent(this, PerfilActivity.class);
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
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
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

        if (id == R.id.opcionAjustes) {
            Toast.makeText(this,"Ajustes", Toast.LENGTH_SHORT).show();
        }
        else if(id==R.id.opcionNotificaciones){
            Toast.makeText(this,"Notificaciones", Toast.LENGTH_LONG).show();
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    public void onBackPressed(){
        if (exit){
            finish();
        }
        else {
            Toast.makeText(this, "Presione de nuevo para Salir.",
                    Toast.LENGTH_SHORT).show();
            exit = true;
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run(){
                    exit = false;
                }
            }, 3 * 1000);
        }
    }

}