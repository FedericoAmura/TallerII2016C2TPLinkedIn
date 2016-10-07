package com.example.android.clientapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
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
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class LoginActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "LOGIN_ACTIVITY";

    private Button botonIngresar;
    private Button botonRegistrar;

    private EditText etUsuario;
    private EditText etPass;

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
    }

    //Funcion a llamar al clickear boton Ingresar.
    public void apretarBotonIngresar(View view){

        final String username = etUsuario.getText().toString().trim();
        final String password = etPass.getText().toString().trim();
        //TODO: validaciones antes de enviar datos
        enviarDatosAlServer(username, password);
    }

    //Funcion a llamar al clickear boton Registrarse.
    public void apretarBotonRegistrar(View view){
        Intent intent = new Intent(this, RegistroActivity.class);
        startActivity(intent);
    }

    public void ingresar(){
        Intent intent = new Intent(this, PestaniasActivity.class);
        startActivity(intent);
        finish();
    }


    public void enviarDatosAlServer(final String username, final String password){
        StringRequest stringRequest = new StringRequest(Request.Method.POST, "http://192.168.0.102:8888/login",
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        String code=null;
                        try {
                            JSONObject jsonObject = new JSONObject(response);
                            code = jsonObject.getString("token"); // TODO: Iria el "code" del json.
                        } catch (JSONException e) { /*hacer algo*/ }
                        if(code.trim().equals("guido123")){
                            Toast.makeText(LoginActivity.this,"Login exitoso.",Toast.LENGTH_LONG).show();
                            ingresar();
                        }else{
                            Toast.makeText(LoginActivity.this,response,Toast.LENGTH_LONG).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(LoginActivity.this,error.toString(),Toast.LENGTH_LONG ).show();
                    }
                }){


            //@Override
            //protected Response<String> parseNetworkResponse(NetworkResponse response){
            //    return super.parseNetworkResponse(response);
            //}

            @Override
            public Map<String,String> getHeaders() throws AuthFailureError {
                HashMap<String, String> params = new HashMap<String,String>();
                //params.put("Authorization", "username="+username+" password="+password);
                params.put("Authorization", "username=guido password=123"); //TODO:Hard para acelerar prueba, usar el de arriba
                return params;
            }

            /*@Override
            protected Map<String, String> getParams() throws AuthFailureError {
                Map<String,String> map = new HashMap<String,String>();
                map.put("username",username);
                map.put("password",password);
                return map;
            }*/
        };

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(stringRequest);
    }
}
