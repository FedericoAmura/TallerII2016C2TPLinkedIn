package com.example.android.clientapp.Modelo;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

/**
 * Created by guidonegri on 04/10/16.
 */

public class Perfil {
    private static final String DEBUG_TAG = "PERFIL";

    // Info personal:
    private String nombre;
    private String apellido;
    private String fecha_nacimiento;
    private String correo;
    private int cant_contactos;
    private int cant_recomendaciones;

    // Resumen:
    private String resumen;

    // Skills:
    private ArrayList<String> skills;

    // Exp Laboral:
    private ArrayList<String> exp_laboral;

    // Ubicacion:
    private String ubicacion;



    public void cargarDesdeJSON(JSONObject object){
        try {
            Log.d(DEBUG_TAG, "Cargando datos del Perfil desde Json");
            nombre = object.getString("first_name");
            //apellido = object.getString("last_name");
            //correo = object.getString("email");
            fecha_nacimiento = object.getString("birth");


        } catch (JSONException e) {
            e.printStackTrace();
        }
    }






    public String getNombre(){
        return nombre;
    }

    public String getApellido(){
        return apellido;
    }

    public String getFechaNacimiento(){
        return fecha_nacimiento;
    }

    public String getCorreo(){
        return correo;
    }

    public int getCantContactos(){
        return cant_contactos;
    }

    public int getCantRecomendaciones() {
        return cant_recomendaciones; }
}
