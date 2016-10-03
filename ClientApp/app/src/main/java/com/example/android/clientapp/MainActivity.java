package com.example.android.clientapp;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;

import android.os.Handler;
import android.support.annotation.BoolRes;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "ConexionHTTP";
    private EditText urlText;
    private TextView textViewRespuesta;
    private Boolean exit = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        urlText = (EditText) findViewById(R.id.editTextUrl);
        textViewRespuesta = (TextView) findViewById(R.id.textViewRespuesta);
    }

    @Override
    public void onBackPressed(){
        if (exit){
            finish();
        }
        else {
            Toast.makeText(this, "Presione de nuevo para Salir.",
                    Toast.LENGTH_SHORT).show();
            exit = true;
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run(){
                    exit= false;
                }
            }, 3 * 1000);
        }
    }

    //Funcion a llamar al clickear boton LoginActivity.
    public void apretarBotonLogin(View view){
        //if (AccessToken.getCurrentAccessToken() == null) {
        //   Todo: ver si lo termino usando y donde exactamente.
        // }
        //Intent intent = new Intent(this, LoginActivity.class); // Descomentar para activity posta de login
        Intent intent = new Intent(this, PestaniasActivity.class);
        startActivity(intent);
    }

    // Funcion a llamar al clickear boton Conectar.
    public void apretarBotonConectar(View view) {
        // Obtengo URL:
        String stringUrl = urlText.getText().toString();

        // Compruebo conectividad a internet:
        ConnectivityManager connMgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()) {
            new DownloadWebpageTask().execute(stringUrl);
        } else {
            textViewRespuesta.setText("No hay conexion disponible.");
        }
    }

    // AsyncTask para crear una tarea fuera del hilo principal.
    private class DownloadWebpageTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {
            try {
                return downloadUrl(urls[0]);
            } catch (IOException e) {
                Log.d(DEBUG_TAG, "No se pudo conectar. Ingresar URL correcta.");
                return "Ingrese una URL correcta.";
            }
        }
        // onPostExecute se muestra el resultado del AsyncTask.
        @Override
        protected void onPostExecute(String result) {
            textViewRespuesta.setText(result);
        }
    }

    // Establece conexion con la URL pasada.
    private String downloadUrl(String myurl) throws IOException {
        InputStream is = null;
        int len = 1000; //Cantidad de caracteres a mostrar de lo obtenido en el GET

        try {
            URL url = new URL(myurl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(2000); //En mseg
            conn.setConnectTimeout(2000); //En mseg
            conn.setRequestMethod("GET");
            conn.setDoInput(true);

            conn.connect();
            int codigoRespuesta = conn.getResponseCode();
            Log.d(DEBUG_TAG, "Codigo de respuesta: " + codigoRespuesta);

            if (codigoRespuesta == 200) { Log.d(DEBUG_TAG, "Conexion establecida correctamente.");}

            is = conn.getInputStream();

            // Convertimos el InputStream a string para mostrarlo en la TextView.
            String contentAsString = convertirInputstringEnString(is, len);
            return contentAsString;

        } finally {
            if (is != null) {
                is.close();
            }
        }
    }

    // Conversor de Inputstring en String
    public String convertirInputstringEnString(InputStream stream, int len) throws IOException, UnsupportedEncodingException {
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }
}
