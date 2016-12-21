package com.example.android.clientapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.Cripto;
import com.example.android.clientapp.utils.GPS;
import com.example.android.clientapp.utils.RequestQueueSingleton;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class RegistroActivity extends AppCompatActivity {
    private final String LOG_TAG = "PESTANIAS_ACTIVITY";
    private static final String DEBUG_TAG = "REGISTRO";
    private static final String FIRST_NAME = "first_name";
    private static final String LAST_NAME = "last_name";
    private static final String BIRTHDAY = "birth";
    private static final String EMAIL = "email";
    private static final String USERNAME = "username";
    private static final String PASSWORD = "password";
    private static final String CITY = "city";
    private static final String LONGITUDE = "longitude";
    private static final String LATITUDE = "latitude";


    private LoginButton loginButton;
    private CallbackManager callbackManager;

    private EditText etNombre;
    private EditText etApellido;
    private EditText etEdad;
    private EditText etCorreo;
    private EditText etNombreUsuario;
    private EditText etPass;
    private EditText etPassRepetido;

    private Button botonCancelar;
    private Button botonEnviar;

    private GPS gps;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registro);

        gps = new GPS(this);

        botonCancelar = (Button) findViewById(R.id.boton_cancelar);
        botonCancelar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonCancelar(v);
            }

        });

        botonEnviar = (Button) findViewById(R.id.botonAceptar);
        botonEnviar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonEnviar(v);
            }

        });


        etNombre = (EditText) findViewById(R.id.campo_nombre);
        etApellido = (EditText) findViewById(R.id.campo_apellido);
        etEdad = (EditText) findViewById(R.id.campo_nacimiento);
        etCorreo = (EditText) findViewById(R.id.campo_correo);
        etNombreUsuario = (EditText) findViewById(R.id.campo_usuario);
        etPass = (EditText) findViewById(R.id.campo_password);
        etPassRepetido = (EditText) findViewById(R.id.campo_password_repetido);

        callbackManager = CallbackManager.Factory.create();

        loginButton = (LoginButton) findViewById(R.id.loginButton);

        loginButton.setReadPermissions(Arrays.asList(
                "public_profile", "email", "user_birthday"));

        loginButton.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                Log.d(DEBUG_TAG, "Login correcto con Facebook.");

                GraphRequest request = GraphRequest.newMeRequest(
                        loginResult.getAccessToken(),
                        new GraphRequest.GraphJSONObjectCallback() {
                            @Override
                            public void onCompleted(JSONObject object, GraphResponse response) {
                                Log.v(DEBUG_TAG, response.toString());

                                if (response.getError() != null) {
                                    Log.e(DEBUG_TAG, "Error con GraphResponse");
                                } else {

                                    try {
                                        Log.d(DEBUG_TAG, "Cargando datos del login Facebook");
                                        String nombre = object.getString(FIRST_NAME);
                                        String apellido = object.getString(LAST_NAME);
                                        String correo = object.getString(EMAIL);
                                        String nacimiento = object.getString("birthday");
                                        etNombre.setText(nombre);
                                        etApellido.setText(apellido);
                                        etCorreo.setText(correo);
                                        etEdad.setText(nacimiento);


                                    } catch (JSONException e) {
                                        e.printStackTrace();
                                    }
                                }

                            }
                        });
                Bundle parameters = new Bundle();
                parameters.putString("fields", "first_name,last_name,email,birthday");
                request.setParameters(parameters);
                request.executeAsync();

            }

            @Override
            public void onCancel() {
                Log.d(DEBUG_TAG, "Login cancelado con Facebook.");
                Toast.makeText(getApplicationContext(), R.string.cancel_login, Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onError(FacebookException error) {
                Log.d(DEBUG_TAG, "Login incorrecto con Facebook.");
                Toast.makeText(getApplicationContext(), R.string.error_login, Toast.LENGTH_SHORT).show();
            }
        });

    }

    @Override
    protected void onStop() {
        super.onStop();
        RequestQueueSingleton.getInstance(this).cancelPendingRequests(LOG_TAG);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }


    private boolean validarDatos(String nombre, String apellido, String edad, String correo, String username,
                                 String pass, String passRepetido){
        boolean ok = true;

        // Campos vacios:
        if (nombre.equals("") || apellido.equals("") || edad.equals("") || correo.equals("") || username.equals("") ||
                pass.equals("") || passRepetido.equals("")) {
            Toast.makeText(RegistroActivity.this,"Debe completar todos los campos.",Toast.LENGTH_LONG).show();
            ok = false;
        }

        // Contrasenas no coinciden:
        if (! pass.equals(passRepetido)) {
            Toast.makeText(RegistroActivity.this,"Passwords no coinciden.",Toast.LENGTH_LONG).show();
            ok = false;
        }

        // GPS desactivado:
        if (! gps.getIsGPSTrackingEnabled()) {
            gps.showSettingsAlert();
        }
        if (! gps.getIsGPSTrackingEnabled()) {
            ok = false;
        }

        return ok;
    }

    //Funcion a llamar al clickear boton Cancelar.
    public void apretarBotonCancelar(View view){
        super.onBackPressed();
    }

    //Funcion a llamar al clickear boton Enviar.
    public void apretarBotonEnviar(View view){
        final String nombre = etNombre.getText().toString();
        final String apellido = etApellido.getText().toString();
        final String edad = etEdad.getText().toString();
        final String correo = etCorreo.getText().toString();
        final String username = etNombreUsuario.getText().toString();
        final String pass = etPass.getText().toString();
        final String passRepetido = etPassRepetido.getText().toString();

        final String passEncriptada = Cripto.encodeString(etPass.getText().toString());
        if (passEncriptada == null) {Log.d(DEBUG_TAG, "Pass null");}


        boolean ok = validarDatos(nombre, apellido, edad, correo, username, pass, passRepetido);
        if (ok) { //TODO: descomentar validaciones
            enviarDatosAlServer(nombre, apellido, edad, correo, username, passEncriptada);
        }
    }

    private void enviarDatosAlServer(final String nombre, final String apellido, final String edad, final String correo,
                                     final String username, final String pass){

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.putOpt(FIRST_NAME, nombre);
            jsonObj.putOpt(LAST_NAME, apellido);
            jsonObj.putOpt(BIRTHDAY, edad);
            jsonObj.putOpt(EMAIL, correo);
            jsonObj.putOpt(USERNAME, username);
            jsonObj.putOpt(PASSWORD, pass);

            // Cargo datos de localizacion:
            if (gps.getIsGPSTrackingEnabled()) {
                gps.getLocation(); // Actualizamos la locacion
                jsonObj.putOpt(LATITUDE, gps.getLatitude());
                jsonObj.putOpt(LONGITUDE, gps.getLongitude());
                jsonObj.putOpt(CITY, gps.getLocality(this));
            }
            else {
                gps.showSettingsAlert();
            }
        } catch (JSONException e) { }

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST, JobifyAPI.getRegistroURL(), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_CREATED){
                            Toast.makeText(RegistroActivity.this,"Usuario creado exitosamente.",Toast.LENGTH_LONG).show();
                            RegistroActivity.super.onBackPressed();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == 422) {
                            Toast.makeText(RegistroActivity.this, "Registro incorrecto. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                    }
                }) {

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
}
