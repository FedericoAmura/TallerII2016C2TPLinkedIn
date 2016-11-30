package com.example.android.clientapp;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.preference.PreferenceManager;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
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
import com.example.android.clientapp.utils.PreferenceHandler;
import com.example.android.clientapp.utils.UserCredentials;

import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.facebook.FacebookSdk.getApplicationContext;

/**
 * Created by guidonegri on 22/11/16.
 */

public class NotifAdapter extends RecyclerView.Adapter<NotifAdapter.PersonViewHolder> {

    public static class PersonViewHolder extends RecyclerView.ViewHolder {

        CardView cv;
        TextView personName;
        TextView personCity;
        ImageView personPhoto;
        ImageButton botonAceptar;
        ImageButton botonDeclinar;
        String senderID;

        UserCredentials credentials = PreferenceHandler.loadUserCredentials(getApplicationContext());
        final String userID = String.valueOf(credentials.getUserID());
        final String token = credentials.getToken();

        int statusCode;

        View itemView;

        PersonViewHolder(View itemView) {
            super(itemView);
            this.itemView = itemView;
            cv = (CardView)itemView.findViewById(R.id.cv);
            personName = (TextView)itemView.findViewById(R.id.person_name);
            personCity = (TextView)itemView.findViewById(R.id.person_city);
            personPhoto = (ImageView)itemView.findViewById(R.id.person_photo);
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Context context = v.getContext();
                    Intent intent = new Intent(context, PerfilAmigoActivity.class);
                    intent.putExtra("userID", senderID);
                    context.startActivity(intent);
                }
            });

            botonAceptar = (ImageButton)itemView.findViewById(R.id.botonAceptar);
            botonAceptar.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick (View v){
                    apretarBotonAceptar();
                }
            });

            botonDeclinar = (ImageButton)itemView.findViewById(R.id.botonDeclinar);
            botonDeclinar.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick (View v){
                    apretarBotonDeclinar();
                }
            });
        }

        private void apretarBotonAceptar(){
            JSONObject obj = new JSONObject();
            JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST, JobifyAPI.getNotificacionURL(userID, senderID), obj,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            if (statusCode == HttpURLConnection.HTTP_OK){
                                Toast.makeText(getApplicationContext(),"Peticion aceptada.",Toast.LENGTH_LONG).show();
                                itemView.setVisibility(View.GONE);
                            }
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            NetworkResponse netResp = error.networkResponse;
                            if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                                Toast.makeText(getApplicationContext(), "Peticion inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                            if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                                Toast.makeText(getApplicationContext(), "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                        }
                    }) {

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

            RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
            requestQueue.add(jsonRequest);
        }

        private void apretarBotonDeclinar(){
            JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.DELETE, JobifyAPI.getNotificacionURL(userID, senderID), null,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            if (statusCode == HttpURLConnection.HTTP_OK){
                                Toast.makeText(getApplicationContext(),"Peticion declinada.",Toast.LENGTH_LONG).show();
                                itemView.setVisibility(View.GONE);
                            }
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            NetworkResponse netResp = error.networkResponse;
                            if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                                Toast.makeText(getApplicationContext(), "Peticion inexistente. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                            if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_FORBIDDEN) {
                                Toast.makeText(getApplicationContext(), "No autorizado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                            }
                        }
                    }) {

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

            RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
            requestQueue.add(jsonRequest);
        }

    }

    List<Amigo> amigos;

    public NotifAdapter(List<Amigo> amigos){
        this.amigos = amigos;
    }

    @Override
    public void onAttachedToRecyclerView(RecyclerView recyclerView) {
        super.onAttachedToRecyclerView(recyclerView);
    }

    @Override
    public PersonViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View v = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.notif_item, viewGroup, false);
        PersonViewHolder pvh = new PersonViewHolder(v);
        return pvh;
    }

    @Override
    public void onBindViewHolder(PersonViewHolder personViewHolder, int i) {
        personViewHolder.personName.setText(amigos.get(i).getNombre());
        personViewHolder.personCity.setText(amigos.get(i).getCiudad());
        Bitmap thumb = amigos.get(i).getFoto();
        if (thumb != null) { personViewHolder.personPhoto.setImageBitmap(thumb); }
        else personViewHolder.personPhoto.setImageResource(R.drawable.ic_user_black);
        //personViewHolder.personPhoto.setImageBitmap(amigos.get(i).getFoto());
        personViewHolder.senderID = amigos.get(i).getUserID();
    }

    @Override
    public int getItemCount() {
        return amigos.size();
    }
}