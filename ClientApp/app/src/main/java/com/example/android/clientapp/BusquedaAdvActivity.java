package com.example.android.clientapp;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.GPS;
import com.example.android.clientapp.utils.JsonUtil;
import com.example.android.clientapp.utils.RequestQueueSingleton;

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Activity para las busquedas altamente parametrizables
 */
public class BusquedaAdvActivity extends SkillJobActivity {
    private final String LOG_TAG = "BUSQUEDA_ADV_ACTIVITY";
    private int statusCode;
    private ProgressBar bar;
    private Switch geoSwitch;
    private CheckBox popSortBox;
    private TextView tvSkills;
    private TextView tvJobs;
    private ArrayList<String> selectedSkills;
    private ArrayList<String> selectedJobs;
    private GPS gps;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_busqueda_adv);

        gps = new GPS(this);
        bar = (ProgressBar) findViewById(R.id.progressBar);
        tvSkills = (TextView) findViewById(R.id.tvSkills);
        tvJobs = (TextView) findViewById(R.id.tvPos);
        popSortBox = (CheckBox) findViewById(R.id.popSort);
        selectedSkills = new ArrayList<String>();
        selectedJobs = new ArrayList<String>();

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

        Button botGeoLoc = (Button) findViewById(R.id.butGeoloc);
        botGeoLoc.setOnClickListener(new View.OnClickListener() {
            @Override
            public  void onClick(View v) {
                if (gps.getIsGPSTrackingEnabled()) {
                    ((TextView)findViewById(R.id.campoLatitud)).setText(Double.toString(gps.getLatitude()));
                    ((TextView)findViewById(R.id.campoLongitud)).setText(Double.toString(gps.getLongitude()));
                    ((TextView)findViewById(R.id.campoDistancia)).setText("1");
                }
                else {
                    gps.showSettingsAlert();
                }
            }
        });

        Button botonSkills = (Button) findViewById(R.id.butSkill);
        botonSkills.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botonSkill();
            }
        });

        Button botonPos = (Button) findViewById(R.id.butPos);
        botonPos.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botonPos();
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

    /**
     * Método que ejecuta la busqueda al ser apretado el boton de búsqueda
     * @param v
     */
    public void buscar(View v) {
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
        buscarYCambiarActivity(JobifyAPI.getAdvBuscquedaURL(selectedSkills, selectedJobs,
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
        jsonRequest.setTag(LOG_TAG);
        RequestQueueSingleton.getInstance(this).addToRequestQueue(jsonRequest);
//        RequestQueue requestQueue = Volley.newRequestQueue(this);
//        requestQueue.add(jsonRequest);
    }

    /**
     * Método llama al apretar el boton para agregar skills
     */
    private void botonSkill() {
        botonJobSkill(SKILLS, selectedSkills, tvSkills);
    }

    /**
     * Método llamado al apretar el boton para agregar job positions
     */
    private void botonPos() {
        botonJobSkill(JOB_POSITIONS, selectedJobs, tvJobs);
    }

    /**
     * Metodo usado al apretar los botones de posicion y skill
     * @param id            String que identifica el tipo (SKILL, JOB_POSITION, etc)
     * @param toAdd         Aqui se agregaran las elecciones y contiene previas elecciones
     * @param tv            Textview donde se mostrara lo seleccionado
     */
    private void botonJobSkill(String id, ArrayList<String> toAdd, TextView tv) {
        if (isInitDict.get(id)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(BusquedaAdvActivity.this);
            String[] items = listDictDesc.get(id);
            final List<String> itemsList = Arrays.asList(listDict.get(id));
            boolean[] checkedItems = new boolean[items.length];
            Arrays.fill(checkedItems, false);
            for (int i = 0; i < checkedItems.length; ++i) {
                String val = itemsList.get(i);
                for (int j = 0; j < toAdd.size(); ++j)
                    if (val.equals(toAdd.get(j))) checkedItems[i] = true;
            }
            toAdd.clear();
            crearCheckList(items, itemsList, checkedItems, builder, tv, toAdd);
        }
    }

    /**
     * Crea un menu estilo checklist
     * @param items             Items que se mostraran para seleccion (display textual)
     * @param itemsList         Items que seran efectivamente selectos y devueltos
     * @param checkedItems      Booleans para las check boxes
     * @param builder           Builder para la ventana
     * @param tv                Textview para mostrar las selecciones
     * @param toAdd             Lista a la cual agregar las selecciones
     */
    private void crearCheckList(String[] items, final List<String> itemsList,
                                final boolean[] checkedItems, AlertDialog.Builder builder,
                                final TextView tv, final ArrayList<String> toAdd) {
        builder.setMultiChoiceItems(items, checkedItems, new DialogInterface.OnMultiChoiceClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                checkedItems[which] = isChecked;
            }
        });
        builder.setCancelable(false);
        builder.setTitle("Seleccione las opciones correspondientes:");
        builder.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                for (int i = 0; i<checkedItems.length; ++i)
                    if (checkedItems[i])
                        toAdd.add(itemsList.get(i));
                setTextView(tv, toAdd);
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

    /**
     * Pone una lista en una text view
     * @param tv            La textview
     * @param data          Lista de strings
     */
    private void setTextView(TextView tv, ArrayList<String> data) {
        tv.setText("");
        if (data.size() <= 0) {
            tv.setVisibility(View.GONE);
            return;
        }
        tv.setVisibility(View.VISIBLE);
        for (int i = 0; i < data.size(); ++i)
            tv.setText(tv.getText() + data.get(i) + "\n");
    }
}
