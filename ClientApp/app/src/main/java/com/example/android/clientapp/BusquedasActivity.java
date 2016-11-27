package com.example.android.clientapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.content.Context;

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

public class BusquedasActivity extends AppCompatActivity {

    private int statusCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_busquedas);

        Button boton = (Button) findViewById(R.id.butPop);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botPop(v);
            }
        });

        boton = (Button) findViewById(R.id.butSkill);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botSkill(v);
            }
        });

        boton = (Button) findViewById(R.id.butPosition);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botPosition(v);
            }
        });

        boton = (Button) findViewById(R.id.butAdv);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                botAdv(v);
            }
        });
    }

    private void botPop(View view) {
        buscarYCambiarActivity(JobifyAPI.getTopTenPopURL());
    }

    private void botSkill(View view) {

    }

    private void botPosition(View view) {

    }

    private void botAdv(View view) {

    }

    /**
     * Hace un get sobre la url, y pasa a la activity que muestra la lista de usuarios resultado
     * @param url                   Url a la cual hacerle GET para obtener json con "users"
     */
    private void buscarYCambiarActivity(final String url) {
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                if (statusCode == HttpURLConnection.HTTP_OK) {
                    ArrayList<String> uIDs = jsonToStringArray(response, "users");
                    Intent intent = new Intent(BusquedasActivity.this, UserListActivity.class);
                    intent.putExtra("userIDs", uIDs);
                    startActivity(intent);
                    finish();
                }
            }
        },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    NetworkResponse netResp = error.networkResponse;
                    if (netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                        Toast.makeText(BusquedasActivity.this, "Error al buscar. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                    }
                }
            }) {

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response) {
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };
        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(jsonRequest);
    }

    /**
     * Dado un json con un campo que tiene un array de strings, devuelve el array de strings
     * @param json                   Json a desmenuzar
     * @param fieldName             Nombre del campo con el array
     * @return                      Un arraylist con los valores
     */
    private ArrayList<String> jsonToStringArray(JSONObject json, String fieldName) {
        ArrayList<String> result = new ArrayList<String>();
        try {
            JSONArray jArray = json.getJSONArray(fieldName);
            for (int i = 0; i < jArray.length(); ++i)
                result.add(jArray.getString(i));
        }
        catch(JSONException e) {
            e.printStackTrace();
        }
        return result;
    }
}
