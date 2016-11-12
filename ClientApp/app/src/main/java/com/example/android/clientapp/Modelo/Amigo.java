package com.example.android.clientapp.Modelo;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;

/**
 * Created by guidonegri on 05/11/16.
 */
public class Amigo {
    private static final String DEBUG_TAG = "AMIGO";
    private static final String NAME = "name";
    private static final String CITY = "city";
    private static final String POPULARIDAD = "popularidad";
    private static final String THUMB = "thumb";


    private String name;
    private String city;
    private int popularidad;
    private String userID;


    private String age;
    private Bitmap foto;
    private String strFoto;

    private String msg = null;

    public Amigo() {
    }

    public void cargarDatosBriefDesdeJSON(JSONObject object){
        try {
            Log.d(DEBUG_TAG, "Cargando datos brief de Amigo desde Json");
            name = object.getString(NAME);
            city = object.getString(CITY);
            popularidad = object.getInt(POPULARIDAD);

            strFoto = object.getString(THUMB);
            Log.d("TEST", "STRfoto: "+ strFoto);
            byte[] decodedString = Base64.decode(strFoto, Base64.NO_WRAP);
            InputStream is = new ByteArrayInputStream(decodedString);
            foto = BitmapFactory.decodeStream(is);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }




    public String getNombre(){return name;}
    public String getCiudad(){return city;} //TODO: de prueba CAMBIAR
    public Bitmap getFoto(){return foto;}
    public int getPopularidad(){return popularidad;}
    public String getUserID(){return userID;}

    public void setUserID(String id) {userID = id;}
}