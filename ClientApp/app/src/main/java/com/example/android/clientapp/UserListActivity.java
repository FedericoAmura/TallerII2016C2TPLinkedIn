package com.example.android.clientapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Gravity;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.Modelo.Amigo;
import com.example.android.clientapp.utils.PreferenceHandler;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;

/**
 * Clase que muestra una lista de usuarios
 * Se espera se pasen los siguientes parametros como extras
 * "userIDs" ArrayList de strings, ids de usuario
 * TODO: Merge con la activity de amigos
 */
public class UserListActivity extends NotifiableActivity {

    private int statusCode;
    private int count = 0;
    private ArrayList<Amigo> usuarios;
    private RecyclerView rv;
    private LinearLayoutManager llm;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recycler_view);
        rv = (RecyclerView) findViewById(R.id.rv);
        llm = new LinearLayoutManager(this);
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        ArrayList<String> uIDs = this.getIntent().getStringArrayListExtra("userIDs");
        if (uIDs.size() <= 0) {
            TextView noResults = new TextView(this);
            noResults.setGravity(Gravity.CENTER);
            noResults.setTextSize(40);
            noResults.setText("No hay resultados");
            ViewGroup topView = (ViewGroup)findViewById(android.R.id.content);
            topView.addView(noResults);
        }
        else if (uIDs != null) {
            cargarUsuarios(uIDs);
        }

        setToolbar();
    }

    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle("Resultados");
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

    private void cargarUsuarios(ArrayList<String> uIDs) {
        usuarios = new ArrayList<Amigo>(uIDs.size());
        for (int i = 0; i < uIDs.size(); ++i) {
            usuarios.add(null);
            cargarUsuario(uIDs.get(i), i, uIDs.size());
        }
    }

    private void cargarUsuario(final String userID, final int num, final int size) {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                JobifyAPI.getContactoBriefURL(userID), null, new Response.Listener<JSONObject>() {
                @Override
                public void onResponse(JSONObject response) {
                    if (statusCode == HttpURLConnection.HTTP_OK) {
                        Amigo usuario = new Amigo();
                        usuario.cargarDatosBriefDesdeJSON(response);
                        usuario.setUserID(userID);
                        usuarios.set(num,usuario);
                        ++count;
                        try {
                            PreferenceHandler.updateUserThumbnail(Integer.valueOf(userID), response.getString("thumb"), getApplicationContext());
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                        if (count == size) {
                            inicializarAdapter();
                        }
                    }
                }
            },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    NetworkResponse netResp = error.networkResponse;
                    if (netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                        Toast.makeText(UserListActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                    }
                }
            }) {

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response) {
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }

    private void inicializarAdapter(){
        RVAdapter adapter = new RVAdapter(usuarios, PerfilAmigoActivity.class);
        rv.setAdapter(adapter);
    }

}