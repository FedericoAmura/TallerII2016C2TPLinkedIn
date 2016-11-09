package com.example.android.clientapp.Fragments;

/**
 * Created by guidonegri on 21/09/16.
 */

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.Modelo.Amigo;
import com.example.android.clientapp.JobifyAPI;
import com.example.android.clientapp.PerfilAmigoActivity;
import com.example.android.clientapp.R;
import com.example.android.clientapp.RVAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import static com.facebook.FacebookSdk.getApplicationContext;

public class RelacionesFragment extends Fragment {

    private static final String CONTACTS = "contacts";

    private static final String USER_ID = "userID";
    private static final String TOKEN = "token";

    private ArrayList<Amigo> amigos;
    private ArrayList<String> amigosID;
    private RecyclerView rv;

    private int statusCode;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.recycler_view, container, false);

        rv = (RecyclerView) view.findViewById(R.id.rv);

        LinearLayoutManager llm = new LinearLayoutManager(getContext());
        rv.setLayoutManager(llm);
        rv.setHasFixedSize(true);

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        final String userID = sharedPref.getString(USER_ID, "");
        final String token = sharedPref.getString(TOKEN, "");

        cargarAmigosIdDelServer(userID, token);
        return view;
    }

    private void inicializarData(){
        amigos = new ArrayList<Amigo>();
        for (String userID : amigosID) {
            cargarAmigosDelServer(userID);
        }
    }

    private void inicializarAdapter(){
        RVAdapter adapter = new RVAdapter(amigos, PerfilAmigoActivity.class);
        rv.setAdapter(adapter);
    }

    private void inicializarAmigosID(JSONObject obj){
        try {
            amigosID = new ArrayList<String>();
            JSONArray jsonContacts = obj.getJSONArray(CONTACTS);
            for (int i = 0; i < jsonContacts.length(); i++) {
                amigosID.add(jsonContacts.getString(i));
            }
            inicializarData();
        } catch(JSONException e) {e.printStackTrace();}
    }

    private void cargarAmigosDelServer(final String userID){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getContactoBriefURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            Amigo amigo = new Amigo();
                            amigo.cargarDesdeJSON(response);
                            amigo.setUserID(userID);
                            amigos.add(amigo);
                            if (amigos.size() == amigosID.size()) { inicializarAdapter(); }
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(getActivity(), "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null) {
                            Toast.makeText(getActivity(),"ESTOY ACA 2:" + netResp.statusCode ,Toast.LENGTH_LONG).show();
                        }
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(getContext());
        requestQueue.add(jsonRequest);

    }


    private void cargarAmigosIdDelServer(final String userID, final String token){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getContactosURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            inicializarAmigosID(response);
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(getActivity(), "UserID inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                            Toast.makeText(getActivity(), "Usuario no autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        if ( netResp != null) {
                            Toast.makeText(getActivity(),"ESTOY ACA 2:" + netResp.statusCode ,Toast.LENGTH_LONG).show();
                        }
                    }
                }){

            @Override
            protected Response<JSONObject> parseNetworkResponse(NetworkResponse response){
                statusCode = response.statusCode;
                return super.parseNetworkResponse(response);
            }

            @Override
            public Map<String,String> getHeaders() throws AuthFailureError {
                HashMap<String, String> params = new HashMap<String,String>();
                params.put("Authorization", "token="+token);
                return params;
            }
        };

        RequestQueue requestQueue = Volley.newRequestQueue(getContext());
        requestQueue.add(jsonRequest);
    }

}