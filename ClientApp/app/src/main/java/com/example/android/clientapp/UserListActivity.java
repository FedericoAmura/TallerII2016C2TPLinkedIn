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
import com.example.android.clientapp.utils.RequestQueueSingleton;
import com.facebook.internal.LockOnGetVariable;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;

/**
 * Clase que muestra una lista de usuarios
 * Se espera se pasen los siguientes parametros como extras
 * "userIDs" ArrayList de strings, ids de usuario
 */
public class UserListActivity extends NotifiableActivity {
    private final String LOG_TAG = "USERLIST_ACTIVITY";
    protected int statusCode;
    protected int count = 0;
    protected ArrayList<Amigo> usuarios;
    protected RecyclerView rv;
    protected LinearLayoutManager llm;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recycler_view);
        rv = (RecyclerView) findViewById(R.id.rv);
        llm = new LinearLayoutManager(this);
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        ArrayList<String> uIDs = this.getIntent().getStringArrayListExtra("userIDs");
        if (uIDs != null && uIDs.size() <= 0) {
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

    @Override
    protected void onStop() {
        super.onStop();
        RequestQueueSingleton.getInstance(this).cancelPendingRequests(LOG_TAG);
    }

    protected String getToolbarTitle() { return "Resultados"; };

    protected void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle(getToolbarTitle());
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

    protected void cargarUsuarios(ArrayList<String> uIDs) {
        usuarios = new ArrayList<Amigo>(uIDs.size());
        for (int i = 0; i < uIDs.size(); ++i) {
            usuarios.add(null);
            cargarUsuario(uIDs.get(i), i, uIDs.size());
        }
    }

    protected void cargarUsuario(final String userID, final int num, final int size) {
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
        jsonRequest.setTag(LOG_TAG);
        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
//        RequestQueue requestQueue = Volley.newRequestQueue(this);
//        requestQueue.add(jsonRequest);
    }

    protected void inicializarAdapter(){
        RVAdapter adapter = new RVAdapter(usuarios, PerfilAmigoActivity.class);
        rv.setAdapter(adapter);
    }

}