package com.example.android.clientapp;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.utils.Cripto;
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

public class LoginActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "LOGIN";
    private static final String USER_ID = "userID";
    private static final String TOKEN = "token";
    private static final String USERNAME = "username";
    private static final String PASSWORD = "password";

    private Button botonIngresar;
    private Button botonRegistrar;

    private EditText etUsuario;
    private EditText etPass;

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        botonIngresar = (Button) findViewById(R.id.boton_ingresar);
        botonRegistrar = (Button) findViewById(R.id.boton_registrar);

        etUsuario = (EditText) findViewById(R.id.campo_usuario);
        etPass = (EditText) findViewById(R.id.campo_password_login);

        botonIngresar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonIngresar(v);
            }

        });

        botonRegistrar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonRegistrar(v);
            }

        });

        cargarDatosUsuario();
    }

    //Cargo datos si el usuario ya se logeo anteriormente:
    private void cargarDatosUsuario(){
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        String username = sharedPref.getString(USERNAME, "");
        String password = sharedPref.getString(PASSWORD, "");
        etUsuario.setText(username);
        etPass.setText(password);
    }

    //Funcion a llamar al clickear boton Ingresar.
    private void apretarBotonIngresar(View view){
        final String username = etUsuario.getText().toString().trim();
        final String password = etPass.getText().toString().trim();
        boolean ok = validarDatos(username, password);
        if (ok) {
        enviarDatosAlServer(username, password);
        }
    }

    private boolean validarDatos(String username, String password){
        boolean ok = true;

        if (username.equals("") || password.equals("")){
            Toast.makeText(LoginActivity.this,"Debe completar todos los campos.",Toast.LENGTH_LONG).show();
            ok = false;
        }

        return ok;
    }

    //Funcion a llamar al clickear boton Registrarse.
    private void apretarBotonRegistrar(View view){
        Intent intent = new Intent(this, RegistroActivity.class);
        startActivity(intent);
    }

    private void ingresar(){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }

    private void enviarDatosAlServer(final String username, final String password){
        JSONObject jsonObj = new JSONObject();

        String fcm_token = PreferenceHandler.getFCMToken(getApplicationContext());
        if (fcm_token != null)
            try {
                jsonObj.putOpt("registration_id", fcm_token);
            } catch (JSONException e) {}

        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST, JobifyAPI.getLoginURL(), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Toast.makeText(LoginActivity.this,"Login exitoso. ¡Bienvenido!",Toast.LENGTH_LONG).show();

                            try {
                                int user_id = response.getInt(USER_ID);
                                String token = response.getString(TOKEN);
                                //Creo preferencia para los chats del usuario:
                                SharedPreferences chatsPref = getApplicationContext().getSharedPreferences("chats_user_"+user_id, MODE_PRIVATE);
                                SharedPreferences.Editor chatEditor = chatsPref.edit();
                                //chatEditor.clear();
                                //chatEditor.commit();

                                //Guardo las preferencias default de usuario:
                                SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences
                                        (getApplicationContext());
                                SharedPreferences.Editor editor = sharedPref.edit();
                                editor.clear();
                                editor.commit();
                                editor.putString(USERNAME, username);
                                editor.putString(PASSWORD, password);
                                editor.putString(USER_ID, String.valueOf(user_id));
                                editor.putString(TOKEN, token);
                                editor.commit();

                                PreferenceHandler.saveUserCredentials(new UserCredentials(user_id, token), getApplicationContext());

                            } catch (JSONException e) { e.printStackTrace(); }
                            ingresar();

                        }else{
                            Toast.makeText(LoginActivity.this,response.toString(),Toast.LENGTH_LONG).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(LoginActivity.this, "Login incorrecto. CODE: " +
                                    netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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
                String pass = Cripto.encodeString(password);
                params.put("Authorization", "username="+username+" password="+pass);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }


}

