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

public class ChatFragment extends Fragment {

    private TextView tv;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_chat, container, false);
        //Button boton = (Button) view.findViewById(R.id.button);
        //Button boton2 = (Button) view.findViewById(R.id.button2);
        //tv = (TextView) view.findViewById(R.id.textView);

        //boton.setOnClickListener(new View.OnClickListener(){
        //    @Override
        //   public void onClick (View v){
        //      tv.setText("CLIK PAPAAAASAAA");
        // }

//        });
        //boton2.setOnClickListener(new View.OnClickListener(){
        //    @Override
        //   public void onClick (View v){
        //      tv.setText("CLIK CRACK");
        //  }

//        });

        return view;
    }
}