package com.example.android.clientapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.Profile;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;

import java.util.Arrays;

public class LoginActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "LOGIN";

    private Button botonIngresar;
    private Button botonRegistrar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        botonIngresar = (Button) findViewById(R.id.boton_ingresar);
        botonRegistrar = (Button) findViewById(R.id.boton_registrar);

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
/*
        tvNombre = (TextView) findViewById(R.id.tvNombre);
        tvApellido = (TextView) findViewById(R.id.tvApellido);
        tvToken = (TextView) findViewById(R.id.tvToken);

        callbackManager = CallbackManager.Factory.create();

        loginButton = (LoginButton) findViewById(R.id.loginButton);

        loginButton.setReadPermissions(Arrays.asList("email"));

        loginButton.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                Log.d(DEBUG_TAG, "Login correcto con Facebook.");

                Profile perfilUsuario = Profile.getCurrentProfile();
                tvNombre.setText(perfilUsuario.getFirstName());
                tvApellido.setText(perfilUsuario.getLastName());
                tvToken.setText(loginResult.getAccessToken().getUserId());
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
    */

    }

    /*
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }
    */

    //Funcion a llamar al clickear boton Ingresar.
    public void apretarBotonIngresar(View view){
        Intent intent = new Intent(this, PestaniasActivity.class);
        startActivity(intent);
        finish();
    }

    //Funcion a llamar al clickear boton Registrarse.
    public void apretarBotonRegistrar(View view){
        Intent intent = new Intent(this, RegistroActivity.class);
        startActivity(intent);
    }
}
