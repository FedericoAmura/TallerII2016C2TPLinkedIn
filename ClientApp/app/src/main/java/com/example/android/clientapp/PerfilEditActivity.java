package com.example.android.clientapp;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.preference.PreferenceManager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
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
import com.example.android.clientapp.utils.AppServerNotification;
import com.example.android.clientapp.utils.GPS;
import com.example.android.clientapp.utils.NotificationLauncher;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;
import com.google.firebase.messaging.RemoteMessage;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.lang.reflect.Array;
import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.facebook.FacebookSdk.getApplicationContext;

public class PerfilEditActivity extends SkillJobActivity {

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
    private static final int SELECT_PHOTO = 100;

    private EditText etNombre;
    private EditText etNacimiento;
    private EditText etCorreo;
    private EditText etResumen;

    private Button botonCancelar;
    private Button botonEnviar;
    private Button btnSkills;
    private Button btnExperiencia;
    private Button botonFoto;

    private ImageView ivFoto;
    private String strFoto;

    private TextView tvSkills;
    private TextView tvExperiencia;

    private GPS gps;
    private RadioGroup rdgGps;
    private boolean editarUbicacion;

    private Perfil perfil;
    private ArrayList<Boolean> skills_bool;
    private ArrayList<Boolean> jobs_bool;

    private JSONArray userSkills;
    private JSONArray userJobs;

    private UserCredentials credentials;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil_edit);

        credentials = PreferenceHandler.loadUserCredentials(getApplicationContext());

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

        tvSkills = (TextView) findViewById(R.id.tvSkills);
        tvExperiencia = (TextView) findViewById(R.id.tvExperiencia);
        userSkills = new JSONArray();
        userJobs = new JSONArray();

        Intent i = getIntent();
        String jsonPerfil = (String) i.getExtras().get("perfil");
        perfil = new Perfil();
        try {
            JSONObject jsonObjPerfil = new JSONObject(jsonPerfil);
            perfil.cargarDesdeJSON(jsonObjPerfil);
            userSkills = jsonObjPerfil.getJSONArray(SKILLS);
            userJobs = jsonObjPerfil.getJSONArray(JOB_POSITIONS);
        } catch (JSONException e) {}
        setTextViewSkill(tvSkills, userSkills);
        setTextViewJobs(tvExperiencia, userJobs);

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

        ivFoto = (ImageView) findViewById(R.id.person_photo);
        ivFoto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                apretarBotonEditarFoto();
            }
        });

        botonFoto = (Button) findViewById(R.id.boton_foto);
        botonFoto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                apretarBotonEditarFoto();
            }
        });

        btnSkills = (Button) findViewById(R.id.btnSkills);
        btnSkills.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               apretarBotonEditarSkills();
            }
        });

        btnExperiencia = (Button) findViewById(R.id.btnExperiencia);
        btnExperiencia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                apretarBotonEditarExperiencia();
            }
        });

        isInitDict.put(SKILLS,false);
        isInitDict.put(JOB_POSITIONS,false);
        inicializarLista(SKILLS, JobifyAPI.getSkillsURL());
        inicializarLista(JOB_POSITIONS, JobifyAPI.getJobsURL());
    }

    private void apretarBotonEditarFoto(){
        Intent photoPickerIntent = new Intent(Intent.ACTION_PICK);
        photoPickerIntent.setType("image/*");
        startActivityForResult(photoPickerIntent, SELECT_PHOTO);
    }

    private void apretarBotonCancelar(View view){
        super.onBackPressed();
    }

    private void apretarBotonEditarSkills(){
        if (isInitDict.get(SKILLS)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(PerfilEditActivity.this);
            String[] items = listDictDesc.get(SKILLS);
            final List<String> itemsList = Arrays.asList(listDict.get(SKILLS));
            boolean[] checkedItems = new boolean[items.length];
            Arrays.fill(checkedItems,false);
            for (int i = 0; i < checkedItems.length; ++i) {
                String skill = itemsList.get(i);
                for (int j = 0; j < userSkills.length(); ++j) {
                    try {
                        if (skill.equals(userSkills.get(j))) checkedItems[i] = true;
                    }
                    catch (JSONException e) { }
                }
            }
            crearCheckListSkills(items, checkedItems, itemsList, builder, tvSkills);
        }
    }


    private void apretarBotonEditarExperiencia() {
        if (isInitDict.get(JOB_POSITIONS)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(PerfilEditActivity.this);
            String[] items = listDictDesc.get(JOB_POSITIONS);
            final List<String> itemsList = Arrays.asList(listDict.get(JOB_POSITIONS));
            boolean[] checkedItems = new boolean[items.length];
            Arrays.fill(checkedItems, false);
            for (int i = 0; i < checkedItems.length; ++i) {
                String skill = itemsList.get(i);
                for (int j = 0; j < userJobs.length(); ++j) {
                    try {
                        if (skill.equals(((JSONObject)userJobs.get(j)).getString("name"))) checkedItems[i] = true;
                    }
                    catch (JSONException e) { }
                }
            }
            crearCheckListJobs(items, checkedItems, itemsList, builder, tvExperiencia);
        }
    }

    private void crearCheckListJobs(String[] items, final boolean[] checkedItems, final List<String> itemsList, AlertDialog.Builder builder, final TextView tv){
        builder.setMultiChoiceItems(items, checkedItems, new DialogInterface.OnMultiChoiceClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which, boolean isChecked) {

                checkedItems[which] = isChecked;

                String currentItem = itemsList.get(which);

                /*Toast.makeText(getApplicationContext(),
                        currentItem + " " + isChecked, Toast.LENGTH_SHORT).show();*/
            }
        });

        builder.setCancelable(false);

        builder.setTitle("Seleccione las opciones correspondientes:");

        builder.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                userJobs = new JSONArray();
                for (int i = 0; i<checkedItems.length; i++){
                    boolean checked = checkedItems[i];
                    if (checked) {
                        try {
                            JSONObject job = new JSONObject();
                            job.putOpt("name", itemsList.get(i));
                            job.putOpt("start", "11/11/1111"); // Hardcodeo para no modificar el server
                            job.putOpt("end", "11/11/1111"); // Idem
                            userJobs.put(job);
                        }
                        catch (JSONException e) { e.printStackTrace(); }
                    }
                }
                setTextViewJobs(tvExperiencia, userJobs);

            }
        });

        /*builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                //hacer algo
            }
        });*/

        builder.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });

        AlertDialog dialog = builder.create();
        dialog.show();

    }

    /**
     * Dado una text view y data, llena la text view con una lista
     */
    private static void setTextViewSkill(TextView tv, JSONArray data) {
        tv.setText("");
        for (int i = 0; i < data.length(); ++i) {
            try {
                tv.setText(tv.getText() + (String)data.get(i) + "\n");
            } catch (JSONException e) { }
        }
    }

    private static void setTextViewJobs(TextView tv, JSONArray data) {
        tv.setText("");
        for (int i = 0; i < data.length(); ++i) {
            try {
                tv.setText(tv.getText() + ((JSONObject)data.get(i)).getString("name") + "\n");
            } catch (JSONException e) { }
        }
    }

    private void crearCheckListSkills(String[] items, final boolean[] checkedItems, final List<String> itemsList, AlertDialog.Builder builder, final TextView tv){
        builder.setMultiChoiceItems(items, checkedItems, new DialogInterface.OnMultiChoiceClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                checkedItems[which] = isChecked;
                String currentItem = itemsList.get(which);
                /*Toast.makeText(getApplicationContext(),
                        currentItem + " " + isChecked, Toast.LENGTH_SHORT).show();*/
            }
        });

        builder.setCancelable(false);

        builder.setTitle("Seleccione las opciones correspondientes:");

        builder.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                userSkills = new JSONArray();
                for (int i = 0; i<checkedItems.length; i++){
                    boolean checked = checkedItems[i];
                    if (checked) {
                        userSkills.put(itemsList.get(i));
                    }
                }
                setTextViewSkill(tvSkills, userSkills);
            }
        });

        /*builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                //hacer algo
            }
        });*/

        builder.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });

        AlertDialog dialog = builder.create();
        dialog.show();

    }

    private void apretarBotonEnviar(View view){
        final String nombre = etNombre.getText().toString();
        final String edad = etNacimiento.getText().toString();
        final String correo = etCorreo.getText().toString();
        final String resumen = etResumen.getText().toString();

        //Bitmap foto = ((BitmapDrawable) ivFoto.getDrawable()).getBitmap();
        /*ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        imageSelected.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOS);
        String strFoto = Base64.encodeToString(byteArrayOS.toByteArray(), Base64.DEFAULT);*/


        /*byte[] decodedString = Base64.decode(strFoto, Base64.NO_WRAP);
        InputStream is = new ByteArrayInputStream(decodedString);
        Bitmap foto2 = BitmapFactory.decodeStream(is);
        ivFoto.setImageBitmap(foto2);*/

        //boolean ok = validarDatos(ver parametros);
        //if (ok) { //TODO: descomentar validaciones
            enviarDatosAlServer(nombre, edad, correo, resumen);
        //}
    }

    private boolean validarDatos(){
        boolean ok = true;

        // Campos vacios:
        if (ok) {
            Toast.makeText(PerfilEditActivity.this,"Debe completar todos los campos.",Toast.LENGTH_LONG).show();
            ok = false;
        }
        return ok;
    }

    private void enviarDatosAlServer(final String nombre, final String edad, final String correo, final String resumen){

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.putOpt(NAME, nombre);;
            jsonObj.putOpt(BIRTHDAY, edad);
            jsonObj.putOpt(EMAIL, correo);
            jsonObj.putOpt(RESUME, resumen);
            jsonObj.putOpt(PHOTO, strFoto);
            jsonObj.put(SKILLS, userSkills);
            jsonObj.putOpt(JOB_POSITIONS, userJobs);

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

        final String userID = String.valueOf(credentials.getUserID());
        final String token = credentials.getToken();

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
                            //PerfilEditActivity.super.onBackPressed();
                            Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
                            startActivity(intent);
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


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent imageReturnedIntent) {
        super.onActivityResult(requestCode, resultCode, imageReturnedIntent);

        switch(requestCode) {
            case SELECT_PHOTO:
                if(resultCode == RESULT_OK){
                    try {
                        Uri selectedImage = imageReturnedIntent.getData();
                        Bitmap imageSelected = decodeUri(selectedImage);
                        InputStream imageStream = getContentResolver().openInputStream(selectedImage);
                        //Bitmap imageSelected = BitmapFactory.decodeStream(imageStream);
                        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                        imageSelected.compress(Bitmap.CompressFormat.PNG, 10, byteArrayOS);
                        strFoto = Base64.encodeToString(byteArrayOS.toByteArray(), Base64.DEFAULT);
                        strFoto = strFoto.replace("\n", "").replace("\r", "");
                        ivFoto.setImageBitmap(imageSelected);
                        Log.d("TEST", "Foto como string:" + strFoto);
                    }
                    catch (FileNotFoundException e) {e.printStackTrace();}
                }
        }
    }

    private Bitmap decodeUri(Uri selectedImage) throws FileNotFoundException {
        BitmapFactory.Options o = new BitmapFactory.Options();
        o.inJustDecodeBounds = true;
        BitmapFactory.decodeStream(getContentResolver().openInputStream(selectedImage), null, o);

        final int REQUIRED_SIZE = 140;

        int width_tmp = o.outWidth, height_tmp = o.outHeight;
        int scale = 1;
        while (true) {
            if (width_tmp / 2 < REQUIRED_SIZE
                    || height_tmp / 2 < REQUIRED_SIZE) {
                break;
            }
            width_tmp /= 2;
            height_tmp /= 2;
            scale *= 2;
        }
        
        BitmapFactory.Options o2 = new BitmapFactory.Options();
        o2.inSampleSize = scale;
        return BitmapFactory.decodeStream(getContentResolver().openInputStream(selectedImage), null, o2);

    }
}
