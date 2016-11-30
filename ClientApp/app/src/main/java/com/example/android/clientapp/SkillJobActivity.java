package com.example.android.clientapp;

import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.Hashtable;

/**
 * Activity que generaliza el codigo que muestra los menues para elegir
 * skills o puestos de trabajo
 */
public class SkillJobActivity extends NotifiableActivity {

    protected int statusCode;
    protected static final String SKILLS = "skills";
    protected static final String JOB_POSITIONS = "job_positions";
    protected Hashtable<String, String[] > listDictDesc;
    protected Hashtable<String, String[] > listDict;
    protected Hashtable<String, Boolean> isInitDict;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        listDictDesc = new Hashtable<String, String[] >();
        listDict = new Hashtable<String, String[] >();
        isInitDict = new Hashtable<String, Boolean>();

    }

    protected void guardarLista(final String jsonFieldName, JSONObject jsonLista) {
        try {
            JSONArray jsonArray = jsonLista.getJSONArray(jsonFieldName);
            int count = jsonArray.length();
            listDictDesc.put(jsonFieldName, new String[count]);
            listDict.put(jsonFieldName, new String[count]);
            for (int i = 0; i < jsonArray.length(); ++i) {
                JSONObject item = jsonArray.getJSONObject(i);
                String result = item.getString("name") + ": " + item.getString("description") + "(Categoria: " + item.getString("category") + ")";
                listDictDesc.get(jsonFieldName)[i] = result;
                listDict.get(jsonFieldName)[i] = item.getString("name");
            }
            isInitDict.put(jsonFieldName,true);
        } catch(JSONException e) {e.printStackTrace();}
    }

    protected void inicializarLista(final String jsonFieldName, final String url){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Log.d("TEST", "Skills obtenidas del server");
                            guardarLista(jsonFieldName, response);
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(SkillJobActivity.this, "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(SkillJobActivity.this, "Usuario no autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }

}
