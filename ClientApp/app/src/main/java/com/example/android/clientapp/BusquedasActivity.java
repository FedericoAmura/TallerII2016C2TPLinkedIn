package com.example.android.clientapp;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.JsonUtil;
import com.example.android.clientapp.utils.RequestQueueSingleton;

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class BusquedasActivity extends SkillJobActivity {
    private final String LOG_TAG = "BUSQUEDAS_ACTIVITY";
    private int statusCode;
    private ProgressBar bar;
    private int selected = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_busquedas);

        Button boton = (Button) findViewById(R.id.butPop);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                buscarYCambiarActivity(JobifyAPI.getTopTenPopURL());
            }
        });

        boton = (Button) findViewById(R.id.butSkill);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botonSkill();
            }
        });

        boton = (Button) findViewById(R.id.butPosition);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botonPos();
            }
        });

        boton = (Button) findViewById(R.id.butAdv);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(BusquedasActivity.this, BusquedaAdvActivity.class);
                startActivity(intent);
                finish();
            }
        });

        bar = (ProgressBar) findViewById(R.id.progressBar);

        setToolbar();
        isInitDict.put(SKILLS,false);
        isInitDict.put(JOB_POSITIONS,false);
        inicializarLista(SKILLS, JobifyAPI.getSkillsURL());
        inicializarLista(JOB_POSITIONS, JobifyAPI.getJobsURL());
    }

    @Override
    protected void onStop() {
        super.onStop();
        RequestQueueSingleton.getInstance(this).cancelPendingRequests(LOG_TAG);
    }

    protected void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) // Habilitar up button
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle("Opciones de Búsqueda");
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
                    Intent intent = new Intent(BusquedasActivity.this, UserListActivity.class);
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
                        Toast.makeText(BusquedasActivity.this, "Error al buscar. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                    }
                }
            }) {

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response) {
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
//        jsonRequest.setTag(LOG_TAG);
//        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }


    /**
     * Método llama al apretar el boton para agregar skills
     */
    private void botonSkill() {
        botonJobSkill(SKILLS, JobifyAPI.getTopTenPopSkillURL());
    }

    /**
     * Método llamado al apretar el boton para agregar job positions
     */
    private void botonPos() {
        botonJobSkill(JOB_POSITIONS, JobifyAPI.getTopTenPopPuestoURL());
    }

    /**
     * Metodo usado al apretar los botones de posicion y skill
     * @param id            String que identifica el tipo (SKILL, JOB_POSITION, etc)
     * @param toAppendUrl       URL a la cual agregarle la eleccion
     */
    private void botonJobSkill(String id, String toAppendUrl) {
        if (isInitDict.get(id)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(BusquedasActivity.this);
            String[] items = listDictDesc.get(id);
            final List<String> itemsList = Arrays.asList(listDict.get(id));
            crearCheckList(items, itemsList, toAppendUrl, builder);
        }
    }

    /**
     * Crea un menu estilo checklist
     * @param items             Items que se mostraran para seleccion (display textual)
     * @param itemsList         Items que seran efectivamente selectos y devueltos
     * @param toAppendUrl       URL a la cual agregarle la eleccion
     * @param builder           Builder para la ventana
     */
    private void crearCheckList(String[] items, final List<String> itemsList,
                                final String toAppendUrl, AlertDialog.Builder builder) {
        builder.setSingleChoiceItems(items, -1, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                selected = which;
            }
        });
        builder.setCancelable(false);
        builder.setTitle("Seleccione la opcion correspondiente:");
        builder.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (selected != -1)
                    buscarYCambiarActivity(toAppendUrl
                            + itemsList.get(selected).replaceAll(" ","%20"));
            }
        });
        builder.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });
        AlertDialog dialog = builder.create();
        dialog.show();
    }
}
