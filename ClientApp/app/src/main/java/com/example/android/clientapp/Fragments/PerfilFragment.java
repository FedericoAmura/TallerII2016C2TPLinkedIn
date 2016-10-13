package com.example.android.clientapp.Fragments;

/**
 * Created by guidonegri on 21/09/16.
 */

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.example.android.clientapp.Modelo.Perfil;
import com.example.android.clientapp.R;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.concurrent.TimeoutException;

public class PerfilFragment extends Fragment {

    //Info personal:
    private TextView tvNombre;
    private TextView tvEdad;
    private TextView tvCorreo;
    private TextView tvFav;
    private TextView tvRelaciones;

    //Resumen:
    private TextView tvResumen;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_perfil, container, false);

        ///////////////
        // PRUEBA JSON (hardcodeado)
        JSONObject obj = new JSONObject();
        try {
            obj.put("first_name", "Carlos Bianchi");
            obj.put("birth", "22/10/1951");
            obj.put("email", "lio_kpo2005@gmail.com");
            obj.put("fav", "100");
            obj.put("relaciones", "200");
        } catch (JSONException e) {}

        Perfil perfil = new Perfil();
        perfil.cargarDesdeJSON(obj);
        tvNombre = (TextView) view.findViewById(R.id.person_name);
        tvNombre.setText(perfil.getNombre());

        tvEdad = (TextView) view.findViewById(R.id.person_age);
        tvEdad.setText(perfil.getFechaNacimiento());

        tvCorreo = (TextView) view.findViewById(R.id.person_email);
        tvCorreo.setText(perfil.getCorreo());

        tvFav = (TextView) view.findViewById(R.id.person_fav);
        tvFav.setText(perfil.getCantRecomendaciones());

        tvRelaciones = (TextView) view.findViewById(R.id.person_relaciones);
        tvRelaciones.setText(perfil.getCantContactos());
        ///////////////

        tvResumen = (TextView) view.findViewById(R.id.tvResumen);
        tvResumen.setText("Creador de Facebook y Twitter");

//        boton.setOnClickListener(new View.OnClickListener(){
  //          @Override
//            public void onClick (View v){
 //               tv.setText("CLIK PAPAAAASAAA");
 //           }

  //      });
        return view;
    }
}