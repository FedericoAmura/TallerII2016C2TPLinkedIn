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

import com.example.android.clientapp.R;

import java.util.concurrent.TimeoutException;

public class PerfilFragment extends Fragment {

    private TextView tvNombre;
    private TextView tvEdad;
    private TextView tvResumen;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_perfil, container, false);
        //Button boton = (Button) view.findViewById(R.id.button);
        tvNombre = (TextView) view.findViewById(R.id.person_name);
        tvNombre.setText("Carlos Bianchi");

        tvEdad = (TextView) view.findViewById(R.id.person_age);
        tvEdad.setText("22/10/1950");

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