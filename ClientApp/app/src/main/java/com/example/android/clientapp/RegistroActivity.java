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

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.Profile;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;

public class RegistroActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "REGISTRO";

    private Button botonCancelar;
    private Button botonEnviar;

    private LoginButton loginButton;
    private CallbackManager callbackManager;

    private EditText etNombre;
    private EditText etApellido;
    private EditText etEdad;
    private EditText etCorreo;

    // ESTAS SON LAS QUE VAN:

    private Button botonIngresar;
    private Button botonResgistrar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registro);

        botonCancelar = (Button) findViewById(R.id.boton_cancelar);
        botonCancelar.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick (View v){
                apretarBotonCancelar(v);
            }

        });


        etNombre = (EditText) findViewById(R.id.campo_nombre);
        etApellido = (EditText) findViewById(R.id.campo_apellido);
        etEdad = (EditText) findViewById(R.id.campo_nacimiento);
        etCorreo = (EditText) findViewById(R.id.campo_correo);

        callbackManager = CallbackManager.Factory.create();

        loginButton = (LoginButton) findViewById(R.id.loginButton);

        loginButton.setReadPermissions(Arrays.asList(
                "public_profile", "email", "user_birthday"));

        loginButton.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                Log.d(DEBUG_TAG, "Login correcto con Facebook.");

                /*Profile perfilUsuario = Profile.getCurrentProfile();
                etNombre.setText(perfilUsuario.getFirstName());
                etApellido.setText(perfilUsuario.getLastName());*/

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
                                        String nombre = object.getString("first_name");
                                        String apellido = object.getString("last_name");
                                        String correo = object.getString("email");
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
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }


    //Funcion a llamar al clickear boton Cancelar.
    public void apretarBotonCancelar(View view){
        super.onBackPressed();
    }
}
