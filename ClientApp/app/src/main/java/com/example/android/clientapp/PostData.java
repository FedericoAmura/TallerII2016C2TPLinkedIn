package com.example.android.clientapp;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

import android.os.AsyncTask;
import android.os.Handler;
import android.util.Log;

import org.json.JSONObject;

public class PostData {
    String jSONString;
    //JSONObject obj;
    Handler handler = new Handler();
    String rta = null;

    public PostData(String jSONString) {
        super();
        this.jSONString = jSONString;
        //this.obj = obj;
    }

    public String getjSONString() {
        return jSONString;
        //return obj;
    }

/*    public void setjSONString(String jSONString) {
        this.jSONString = jSONString;
    }*/

    public String timer() {
        /*new Thread(new Runnable() {
            @Override
            public void run() {
                boolean run = true;
                while (run) {
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {*/
                            //MyAsyncTask myAsTa = new MyAsyncTask();
                            //myAsTa.execute(jSONString);
                            //rta = myAsTa.getRta();
                      /*  }
                    }, 5000);
                    run = false;
                }
            }
        }).start();
        Log.d("POST_DATA", "LA SALIDA EN TIMER FUE: " + rta);
        return rta;*/
        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                MyAsyncTask myAsTa = new MyAsyncTask();
                myAsTa.execute(jSONString);
                rta = myAsTa.getRta();
                while (rta == null) {
                    Log.d("POST_DATA", "LA SALIDA EN WHILE FUE FUE: " + rta);
                    rta = myAsTa.getRta();
                }
            }
        }, 5000);
        Log.d("POST_DATA", "LA SALIDA EN TIMER FUE: " + rta);
        return rta;
    }

    class MyAsyncTask extends AsyncTask<String, Void, String> {

        private String rta2;

        @Override
        protected String doInBackground(String... params) {
            InputStream is = null;
            int len = 1000;
            try {

                URL myUrl = new URL("http://192.168.0.102:8888/login");
                HttpURLConnection myConnection = (HttpURLConnection) myUrl
                        .openConnection();
                myConnection.setRequestMethod("POST");
                myConnection.setDoOutput(true);
                myConnection.setUseCaches(false);
                myConnection.setConnectTimeout(2000);
                myConnection.setReadTimeout(2000);
                myConnection.setRequestProperty("Content-Type",
                        "application/json");
                myConnection.setRequestProperty("Authorization", "username=guido password=123");
                myConnection.connect();

                int codigoRespuesta = myConnection.getResponseCode();
                Log.d("POST_DATA", "Codigo de respuesta: " + codigoRespuesta);

                if (codigoRespuesta == 200) { Log.d("POST_DATA", "Conexion establecida correctamente.");}
                else { Log.d("POST_DATA", "Conexion incorrecta."); }

                is = myConnection.getInputStream();

                // Convertimos el InputStream a string para mostrarlo en la TextView.
                String contentAsString = convertirInputstringEnString(is, len);
                //return contentAsString;
                return "HOLA";

                // create data output stream
                //DataOutputStream wr = new DataOutputStream(
                //        myConnection.getOutputStream());
                // write to the output stream from the string
                //wr.writeBytes(jSONString);
                //wr.close();
            } catch (IOException e) {

                e.printStackTrace();
                return null;
            }

        }

        @Override
        protected void onPostExecute(String respuesta) {
            super.onPostExecute(respuesta);
            Log.d("POST_DATA", "RESPUESTA EN POSTEXECUTE " + respuesta);
            rta2 = respuesta;
        }

        public String getRta(){
            return rta2;
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