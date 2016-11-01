package com.example.android.clientapp.Fragments;

/**
 * Created by guidonegri on 21/09/16.
 */

import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Base64;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.android.clientapp.JobifyAPI;
import com.example.android.clientapp.Modelo.Perfil;
import com.example.android.clientapp.PestaniasActivity;
import com.example.android.clientapp.R;
import com.example.android.clientapp.RegistroActivity;
import com.example.android.clientapp.VolleyRequest;

import android.view.ViewGroup.LayoutParams;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.concurrent.TimeoutException;

import static com.facebook.FacebookSdk.getApplicationContext;

public class PerfilFragment extends Fragment {

    public Perfil perfil;

    //Info personal:
    private TextView tvNombre;
    private ImageView ivFoto;
    private TextView tvEdad;
    private TextView tvCorreo;
    private TextView tvFav;
    private TextView tvRelaciones;
    private TextView tvUbicacion;

    //Resumen:
    private TextView tvResumen;

    private int statusCode;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_perfil, container, false);

        ///////////////
        // PRUEBA JSON (hardcodeado)
        /*JSONObject obj = new JSONObject();
        try {
            obj.put("first_name", "Carlos Bianchi");
            obj.put("birth", "22/10/1951");
            obj.put("email", "lio_kpo2005@gmail.com");
            obj.put("fav", "100");
            obj.put("relaciones", "200");
        } catch (JSONException e) {}

        Perfil perfil = new Perfil();
        perfil.cargarDesdeJSON(obj);
        cargarPerfil(view, perfil);
        ////////
        */

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        String userID = sharedPref.getString("userID", "");

        cargarDatosDelServer(view, userID);
        return view;
    }

    private void cargarDatosDelServer(final View view, final String userID){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getPerfilURL(userID), null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            perfil = new Perfil();
                            perfil.cargarDesdeJSON(response);
                            cargarPerfil(view, perfil);
                            //Toast.makeText(getActivity(),"Perfil cargado.",Toast.LENGTH_LONG).show();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(getActivity(), "Perfil no cargado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
                        }
                        /*if ( netResp != null) {
                            Toast.makeText(getActivity(),"ESTOY ACA 2:" + netResp.statusCode ,Toast.LENGTH_LONG).show();
                        }*/
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

    public void cargarPerfil(View view, Perfil perfil){
        ivFoto = (ImageView) view.findViewById(R.id.person_photo);
        ivFoto.setImageBitmap(perfil.getFoto());

        tvNombre = (TextView) view.findViewById(R.id.person_name);
        tvNombre.setText(perfil.getNombre());

        tvFav = (TextView) view.findViewById(R.id.person_fav);
        tvFav.setText(perfil.getCantRecomendaciones());

        tvRelaciones = (TextView) view.findViewById(R.id.person_relaciones);
        tvRelaciones.setText(perfil.getCantContactos());

        //tvEdad = (TextView) view.findViewById(R.id.person_age);
        //tvEdad.setText(String.valueOf(perfil.getSizeExp()));

        // Tv dinamicos de Skills:
        LinearLayout llSkills = (LinearLayout) view.findViewById(R.id.llSkills);
        for (int i = 0; i < perfil.getSizeSkills(); i++ ) {
            TextView tvSkill = new TextView(getApplicationContext());
            tvSkill.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT,
                    LayoutParams.WRAP_CONTENT));
            tvSkill.setText(perfil.getSkills(i));
            tvSkill.setTextColor(Color.GRAY);
            llSkills.addView(tvSkill);
        }

        // Tv dinamicos de ExpLaboral:
        LinearLayout llExpLaboral = (LinearLayout) view.findViewById(R.id.llExpLaboral);
        for (int i = 0; i < perfil.getSizeExp(); i++ ) {
            for (int j = 0; j < 3; j++) {
                TextView tvExpLaboral = new TextView(getApplicationContext());
                tvExpLaboral.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT,
                        LayoutParams.WRAP_CONTENT));
                switch (j) {
                    case (0):
                        tvExpLaboral.setText("- Lugar: " + perfil.getExpLaboralNombre(i));
                        break;
                    case (1):
                        tvExpLaboral.setText("     Desde: " + perfil.getExpLaboralInicio(i));
                        break;
                    case (2):
                        tvExpLaboral.setText("     Hasta: " + perfil.getExpLaboralFin(i));
                        break;
                }
                tvExpLaboral.setTextColor(Color.GRAY);
                llExpLaboral.addView(tvExpLaboral);
            }
        }

        tvResumen = (TextView) view.findViewById(R.id.tvResumen);
        tvResumen.setText(perfil.getResumen());

        tvUbicacion = (TextView) view.findViewById(R.id.tvUbicacion);
        tvUbicacion.setText(perfil.getCiudad());
    }
}