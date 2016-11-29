package com.example.android.clientapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.JsonUtil;

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;

/**
 * Activity para las busquedas altamente parametrizables
 */
public class BusquedaAdvActivity extends NotifiableActivity {

    private int statusCode;
    private ProgressBar bar;
    private Switch geoSwitch;
    private CheckBox popSortBox;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_busqueda_adv);

        bar = (ProgressBar) findViewById(R.id.progressBar);
        popSortBox = (CheckBox) findViewById(R.id.popSort);

        geoSwitch = (Switch) findViewById(R.id.switchGeo);
        geoSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    findViewById(R.id.butGeoloc).setVisibility(View.VISIBLE);
                    findViewById(R.id.campoLatitud).setVisibility(View.VISIBLE);
                    findViewById(R.id.campoLongitud).setVisibility(View.VISIBLE);
                    findViewById(R.id.campoDistancia).setVisibility(View.VISIBLE);
                } else {
                    findViewById(R.id.butGeoloc).setVisibility(View.GONE);
                    findViewById(R.id.campoLatitud).setVisibility(View.GONE);
                    findViewById(R.id.campoLongitud).setVisibility(View.GONE);
                    findViewById(R.id.campoDistancia).setVisibility(View.GONE);
                }
            }
        });

        Button botonBuscar = (Button) findViewById(R.id.butBuscar);
        botonBuscar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                buscar(v);
            }
        });

        setToolbar();
    }

    protected void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle("Búsqueda Avanzada");
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

    public void buscar(View v) {
        ArrayList<String> skills = new ArrayList<String>();
        ArrayList<String> puestos = new ArrayList<String>();
        String origenLongitud = "";
        String origenLatitud = "";
        String maxDist = Double.toString(Double.POSITIVE_INFINITY);
        if (geoSwitch.isChecked()) {
            EditText eLong = (EditText)findViewById(R.id.campoLatitud);
            EditText eLat = (EditText)findViewById(R.id.campoLongitud);
            EditText eDist = (EditText)findViewById(R.id.campoDistancia);
            if (eLong.getText().length() > 0)origenLongitud = eLong.getText().toString();
            if (eLat.getText().length() > 0)origenLatitud = eLat.getText().toString();
            if (eDist.getText().length() > 0)maxDist = eDist.getText().toString();
        }
        boolean popSort = popSortBox.isChecked();
        buscarYCambiarActivity(JobifyAPI.getAdvBuscquedaURL(skills, puestos,
                origenLongitud, origenLatitud, maxDist, popSort));
    }

    /**
     * Hace un get sobre la url, y pasa a la activity que muestra la lista de usuarios resultado
     * @param url                   Url a la cual hacerle GET para obtener json con "users"
     */
    private void buscarYCambiarActivity(final String url) {
        bar.setVisibility(View.VISIBLE);
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                if (statusCode == HttpURLConnection.HTTP_OK) {
                    bar.setVisibility(View.INVISIBLE);
                    ArrayList<String> uIDs = JsonUtil.jsonToStringArray(response, "users");
                    Intent intent = new Intent(BusquedaAdvActivity.this, UserListActivity.class);
                    intent.putExtra("userIDs", uIDs);
                    startActivity(intent);
                    finish();
                }
            }
        },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        bar.setVisibility(View.INVISIBLE);
                        NetworkResponse netResp = error.networkResponse;
                        if (netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(BusquedaAdvActivity.this, "Error al buscar. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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
}
