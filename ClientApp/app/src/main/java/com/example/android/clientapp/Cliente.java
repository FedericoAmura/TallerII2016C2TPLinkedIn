package com.example.android.clientapp;

/**
 * Created by guidonegri on 30/08/16.
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class Cliente {

    public void enviarGet() {
        try {

            URL url = new URL(
                    "http://localhost:3000");
            //"http://localhost:3000/prueba");
            //Si corren el Shared Server 2 van a ver una salida mas linda y pueden probar esta URL tmb comentando la otra.
            HttpURLConnection conexion = (HttpURLConnection) url.openConnection();
            conexion.setRequestMethod("GET");

            if (conexion.getResponseCode() != 200) {
                System.out.println("Fallo la conexion... \n");
                throw new RuntimeException("ERROR: codigo de error HTTP: " + conexion.getResponseCode());
            } else {
                System.out.println("Conexion correcta... \n");
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conexion.getInputStream())));

            String output;
            System.out.println("Output del servidor... \n");
            while ((output = br.readLine()) != null) {

                System.out.println(output);
            }

            conexion.disconnect();

        } catch (MalformedURLException e) {

            e.printStackTrace();

        } catch (IOException e) {

            e.printStackTrace();

        }
    }

}