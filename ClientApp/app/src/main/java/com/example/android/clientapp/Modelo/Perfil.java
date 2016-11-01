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
 * Created by guidonegri on 04/10/16.
 */

public class Perfil {
    private static final String DEBUG_TAG = "PERFIL";
    private static final String NAME = "name";
    private static final String CITY = "city";
    private static final String CONTACTS = "contacts";
    private static final String POPULARIDAD = "popularidad";
    private static final String RESUME = "resume";
    private static final String PHOTO = "photo";
    private static final String SKILLS = "skills";
    private static final String JOB_POSITIONS = "job_positions";
    private static final String START = "start";
    private static final String END = "end";

    // Info personal:
    private String nombre;
    private Bitmap foto;
    private String cantContactos;
    private String cantRecomendaciones;

    // Resumen:
    private String resumen;

    // Skills:
    private int sizeSkills;
    private ArrayList<String> skills;

    // Exp Laboral:
    private int sizeExp;
    private ArrayList<JSONObject> expLaboral;

    // Ubicacion:
    private String ciudad;



    public void cargarDesdeJSON(JSONObject object){
        try {
            Log.d(DEBUG_TAG, "Cargando datos del Perfil desde Json");
            nombre = object.getString(NAME);
            ciudad = object.getString(CITY);
            cantContactos = object.getString(CONTACTS);
            cantRecomendaciones = object.getString(POPULARIDAD);
            resumen = object.getString(RESUME);

            String strFoto = object.getString(PHOTO);
            byte[] decodedString = Base64.decode(strFoto, Base64.NO_WRAP);
            InputStream is = new ByteArrayInputStream(decodedString);
            foto = BitmapFactory.decodeStream(is);

            JSONArray jsonSkills = object.getJSONArray(SKILLS);
            sizeSkills = jsonSkills.length();
            skills = new ArrayList<String>();
            Log.d("SKILL SIZE:", ""+sizeSkills );
            for (int i = 0; i < sizeSkills; i++){
                skills.add(jsonSkills.getString(i));
                Log.d("SKILL", jsonSkills.getString(i));
            }


            JSONArray jsonJobPositions = object.getJSONArray(JOB_POSITIONS);
            sizeExp = jsonJobPositions.length();
            expLaboral = new ArrayList<JSONObject>();
            Log.d("EXP SIZE:", ""+sizeExp );
            for (int i = 0; i < sizeExp; i++){
                try {
                    JSONObject objJsonExp =  jsonJobPositions.getJSONObject(i);
                    expLaboral.add(objJsonExp);
                    Log.d("EXP", objJsonExp.getString(NAME));
                }
                catch (JSONException e) {e.printStackTrace();}
            }


        } catch (JSONException e) {
            e.printStackTrace();
        }
    }


    public String getNombre(){
        return nombre;
    }

    public Bitmap getFoto(){
        return foto;
    }

    public String getCantContactos(){
        return cantContactos;
    }

    public String getCantRecomendaciones() {
        return cantRecomendaciones; }

    public String getResumen() { return resumen; }

    public String getCiudad() {
        return ciudad; }

    public int getSizeSkills(){
        return sizeSkills;
    }

    public String getSkills(int i) { return skills.get(i); }

    public int getSizeExp(){
        return sizeExp;
    }

    public String getExpLaboralNombre(int i) {
        try {
            return expLaboral.get(i).getString(NAME);
        } catch (JSONException e) {}
        return null;
    }

    public String getExpLaboralInicio(int i) {
        try {
            return expLaboral.get(i).getString(START);
        } catch (JSONException e) {}
        return null;
    }

    public String getExpLaboralFin(int i) {
        try {
            return expLaboral.get(i).getString(END);
        } catch (JSONException e) {}
        return null;
    }
}
