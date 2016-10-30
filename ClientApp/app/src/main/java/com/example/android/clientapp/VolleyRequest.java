package com.example.android.clientapp;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by guidonegri on 28/10/16.
 */

public class VolleyRequest {

    private Context context;
    private String url;
    private int metodo;
    private String header;
    private JSONObject jsonAlServer;
    private JSONObject jsonDesdeServer;
    private int statusCode;

    public VolleyRequest(Context context, int metodo, String url, JSONObject json, String header) {
        this.metodo = metodo;
        this.url = url;
        this.jsonAlServer = json;
        this.jsonDesdeServer = null;
        this.context = context;
        this.header = header;
    }

    public void crearRequest() {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(metodo, url, jsonAlServer,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("LOOOOOOOOOOOOOOOOOG", "CODE ON:" + statusCode);
                        jsonDesdeServer = response;
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        statusCode = netResp.statusCode;
                        Log.d("LOOOOOOOOOOOOOOOOOG", "CODE ERROR: " + statusCode);
                    }
                }) {


            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response) {
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }

        };

        RequestQueue requestQueue = Volley.newRequestQueue(context);
        requestQueue.add(jsonRequest);
    }

    public JSONObject getJsonDesdeServer(){
        return jsonDesdeServer;
    }

    public int getStatusCode(){
        return statusCode;
    }
}


