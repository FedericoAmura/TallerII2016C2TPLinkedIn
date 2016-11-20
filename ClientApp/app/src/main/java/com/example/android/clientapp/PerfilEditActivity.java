package com.example.android.clientapp;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
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
import com.example.android.clientapp.utils.GPS;
import com.example.android.clientapp.utils.NotificationEvent;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class PerfilEditActivity extends AppCompatActivity {
    private EventBus bus = EventBus.getDefault();

    private static final String DEBUG_TAG = "EDITAR_PERFIL";
    private static final String NAME = "name";
    private static final String BIRTHDAY = "birth";
    private static final String EMAIL = "email";
    private static final String CITY = "city";
    private static final String LONGITUDE = "longitude";
    private static final String LATITUDE = "latitude";
    private static final String RESUME = "resume";
    private static final String PHOTO = "photo";
    private static final String SKILLS = "skills";
    private static final String JOB_POSITIONS = "job_positions";

    private static final String USER_ID = "userID";
    private static final String TOKEN = "token";

    private EditText etNombre;
    private EditText etNacimiento;
    private EditText etCorreo;
    private EditText etResumen;

    private Button botonCancelar;
    private Button botonEnviar;
    private Button btnSkills;
    private Button btnExperiencia;

    private TextView tvSkills;
    private TextView tvExperiencia;

    private GPS gps;
    private RadioGroup rdgGps;
    private boolean editarUbicacion;

    private Perfil perfil;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil_edit);

        gps = new GPS(this);
        editarUbicacion = false;

        botonCancelar = (Button) findViewById(R.id.boton_cancelar);
        botonCancelar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonCancelar(v);
            }
        });

        botonEnviar = (Button) findViewById(R.id.boton_enviar);
        botonEnviar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonEnviar(v);
            }
        });


        Intent i = getIntent();
        String jsonPerfil = (String) i.getExtras().get("perfil");
        perfil = new Perfil();
        try {
            JSONObject jsonObjPerfil = new JSONObject(jsonPerfil);
            perfil.cargarDesdeJSON(jsonObjPerfil);
        } catch (JSONException e) {}

        etNombre = (EditText) findViewById(R.id.campo_nombre);
        etNombre.setText(perfil.getNombre());

        etNacimiento = (EditText) findViewById(R.id.campo_nacimiento);
        etNacimiento.setText(perfil.getNacimiento());

        etCorreo = (EditText) findViewById(R.id.campo_correo);
        etCorreo.setText(perfil.getCorreo());

        etResumen = (EditText) findViewById(R.id.campo_resumen);
        etResumen.setText(perfil.getResumen());

        rdgGps = (RadioGroup)findViewById(R.id.rdgGrupo);
        rdgGps.check(R.id.gps_no);
        rdgGps.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener(){
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if (checkedId == R.id.gps_si){
                    editarUbicacion = true;
                }else if (checkedId == R.id.gps_no){
                    editarUbicacion = false;
                }
            }

        });

        btnSkills = (Button) findViewById(R.id.btnSkills);
        tvSkills = (TextView) findViewById(R.id.tvSkills);
        btnSkills.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               apretarBotonEditarSkills();
            }
        });

        btnExperiencia = (Button) findViewById(R.id.btnExperiencia);
        tvExperiencia = (TextView) findViewById(R.id.tvExperiencia);
        btnExperiencia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                apretarBotonEditarExperiencia();
            }
        });

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
        if (type == 1 || type == 2) //NEW MESSAGE OR FRIEND REQUEST TODO
            NotificationLauncher.launch(this, remoteMessage);
    }

    @Override
    protected void onStop() {
        super.onStop();
        bus.unregister(this);
    }


    private void apretarBotonCancelar(View view){
        super.onBackPressed();
    }

    public void apretarBotonEditarUbicacion(View view){
        if (editarUbicacion == false) {
            editarUbicacion = true;
            Toast.makeText(getApplicationContext(),"Ubicacion actualizada. Envie d", Toast.LENGTH_SHORT).show();
        }
        else { editarUbicacion = false; }
    }



    private void apretarBotonEditarSkills(){
        AlertDialog.Builder builder = new AlertDialog.Builder(PerfilEditActivity.this);

        String[] items = new String[]{
                "C",
                "Java",
                "SmallTalk",
        };

        final boolean[] checkedItems = new boolean[]{
                false,
                false,
                false
        };

        final List<String> itemsList = Arrays.asList(items);

        crearCheckList(items, checkedItems, itemsList, builder, tvSkills);

    }


    private void apretarBotonEditarExperiencia(){
        AlertDialog.Builder builder = new AlertDialog.Builder(PerfilEditActivity.this);

        String[] items = new String[]{
                "Desarrollador",
                "Manager",
                "Disenador",
        };

        final boolean[] checkedItems = new boolean[]{
                false,
                false,
                false
        };

        final List<String> itemsList = Arrays.asList(items);

        crearCheckList(items, checkedItems, itemsList, builder, tvExperiencia);

    }

    private void crearCheckList(String[] items, final boolean[] checkedItems, final List<String> itemsList, AlertDialog.Builder builder, final TextView tv){
        builder.setMultiChoiceItems(items, checkedItems, new DialogInterface.OnMultiChoiceClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which, boolean isChecked) {

                checkedItems[which] = isChecked;

                String currentItem = itemsList.get(which);

                Toast.makeText(getApplicationContext(),
                        currentItem + " " + isChecked, Toast.LENGTH_SHORT).show();
            }
        });

        builder.setCancelable(false);

        builder.setTitle("Seleccione las opciones correspondientes:");

        builder.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                tv.setText("Ha seleccionado: \n");
                for (int i = 0; i<checkedItems.length; i++){
                    boolean checked = checkedItems[i];
                    if (checked) {
                        tv.setText(tv.getText() + itemsList.get(i) + "\n");
                    }
                }
            }
        });

        builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                //hacer algo
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


    //Funcion a llamar al clickear boton Cancelar.
    private void apretarBotonEnviar(View view){
        final String nombre = etNombre.getText().toString();
        final String edad = etNacimiento.getText().toString();
        final String correo = etCorreo.getText().toString();
        final String resumen = etResumen.getText().toString();

        //boolean ok = validarDatos(nombre, apellido, edad, correo, username, pass, passRepetido);
        //if (ok) { //TODO: descomentar validaciones
            enviarDatosAlServer(nombre, edad, correo, resumen);
        //}
    }

    private void enviarDatosAlServer(final String nombre, final String edad, final String correo, final String resumen){

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.putOpt(NAME, nombre);;
            jsonObj.putOpt(BIRTHDAY, edad);
            jsonObj.putOpt(EMAIL, correo);
            jsonObj.putOpt(RESUME, resumen);
            //jsonObj.putOpt(PHOTO, "");
            JSONArray arr = new JSONArray();
            arr.put("java");
            jsonObj.put(SKILLS, arr);
            JSONArray arr2 = new JSONArray();
            JSONObject obj = new JSONObject();
            obj.putOpt("name", "Google");
            obj.putOpt("start", "12/10/2012");
            obj.putOpt("end", "10/10/2014");
            arr2.put(obj);

            jsonObj.putOpt(JOB_POSITIONS, arr2);

            // Cargo datos de localizacion:
            if (editarUbicacion) {
                if (gps.getIsGPSTrackingEnabled()) {
                    jsonObj.putOpt(LATITUDE, gps.getLatitude());
                    jsonObj.putOpt(LONGITUDE, gps.getLongitude());
                    jsonObj.putOpt(CITY, gps.getLocality(this));
                } else {
                    gps.showSettingsAlert();
                }
            } else {
                jsonObj.putOpt(LATITUDE, perfil.getLatitud());
                jsonObj.putOpt(LONGITUDE, perfil.getLongitud());
                jsonObj.putOpt(CITY, perfil.getCiudad());
            }
        } catch (JSONException e) { }

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        final String userID = sharedPref.getString(USER_ID, "");
        final String token = sharedPref.getString(TOKEN, "");

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.PUT, JobifyAPI.getPerfilURL(userID), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {

                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == 422) {
                            Toast.makeText(PerfilEditActivity.this, "Skills o positions invalidos. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(PerfilEditActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(PerfilEditActivity.this, "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if (netResp == null) {
                            Toast.makeText(PerfilEditActivity.this,"Perfil actualizado exitosamente.",Toast.LENGTH_LONG).show();
                            PerfilEditActivity.super.onBackPressed();
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
                params.put("Authorization", "token="+token);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }




}
