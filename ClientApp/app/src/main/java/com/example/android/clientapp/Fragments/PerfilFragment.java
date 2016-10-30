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
import android.widget.Adapter;
import android.widget.ArrayAdapter;
import android.widget.Button;
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

import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.concurrent.TimeoutException;

import static com.facebook.FacebookSdk.getApplicationContext;

public class PerfilFragment extends Fragment {

    //Info personal:
    private TextView tvNombre;
    private TextView tvEdad;
    private TextView tvCorreo;
    private TextView tvFav;
    private TextView tvRelaciones;
    private ListView lvSkills;

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
        Toast.makeText(getActivity(),"URL:"+ JobifyAPI.getPerfilURL(userID),Toast.LENGTH_LONG).show();

        cargarDatosDelServer(view, userID);
        return view;
    }

    private void cargarDatosDelServer(final View view, String userID){
        JSONObject jsonObj = new JSONObject();
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET, JobifyAPI.getPerfilURL(userID), jsonObj,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (statusCode == HttpURLConnection.HTTP_OK){
                            //TODO: las 3 lineas de abajo son las que van.
                            //Perfil perfil = new Perfil();
                            //perfil.cargarDesdeJSON(response);
                            //cargarPerfil(view, perfil);
                            Toast.makeText(getActivity(),"Perfil cargado.",Toast.LENGTH_LONG).show();
                        }
                        else { Toast.makeText(getActivity(),"ESTOY ACA 1." + response.toString(),Toast.LENGTH_LONG).show(); }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse netResp = error.networkResponse;
                        if ( netResp != null && netResp.statusCode == HttpURLConnection.HTTP_NOT_FOUND) {
                            Toast.makeText(getActivity(), "Perfil no cargado. CODE: " + netResp.statusCode, Toast.LENGTH_LONG).show(); //Todo: cambiar mensaje
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

        RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext()); //Todo: VER SI FUNCIONA EL GET CONTEXT
        requestQueue.add(jsonRequest);
    }

    public void cargarPerfil(View view, Perfil perfil){
        tvNombre = (TextView) view.findViewById(R.id.person_name);
        tvNombre.setText(perfil.getNombre());

        /*
        tvEdad = (TextView) view.findViewById(R.id.person_age);
        tvEdad.setText(perfil.getFechaNacimiento());

        tvCorreo = (TextView) view.findViewById(R.id.person_email);
        tvCorreo.setText(perfil.getCorreo());

        tvFav = (TextView) view.findViewById(R.id.person_fav);
        tvFav.setText(perfil.getCantRecomendaciones());

        tvRelaciones = (TextView) view.findViewById(R.id.person_relaciones);
        tvRelaciones.setText(perfil.getCantContactos());

        tvResumen = (TextView) view.findViewById(R.id.tvResumen);
        tvResumen.setText("Creador de Facebook y Twitter");

        LinearLayout llSkills = (LinearLayout) view.findViewById(R.id.llSkills);

        // Add textview 1
        for (int i = 0; i<2; i++ ) {
            TextView textView1 = new TextView(getApplicationContext());
            textView1.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT,
                    LayoutParams.WRAP_CONTENT));
            textView1.setText("Java");
            llSkills.addView(textView1);
        }
        */

        /*lvSkillss.setHasFixedSize(true);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getApplicationContext(), LinearLayoutManager.VERTICAL,false);
        lvSkillss.setLayoutManager(layoutManager);

        ArrayList<String> array = new ArrayList<>();
        array.add("Java");
        array.add("C++");
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(getApplicationContext(), array);
        lvSkillss.setAdapter(adapter);
        //adapter.notifyDataSetChanged();*/
    }
}