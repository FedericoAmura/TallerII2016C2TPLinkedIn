package com.example.android.clientapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TextView textView;
    private Button buttonConnect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        buttonConnect = (Button) findViewById(R.id.button);
        textView = (TextView) findViewById(R.id.textView);

        textView.setVisibility(View.INVISIBLE);

        buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // TODO connect to appserver
                // if (connected)
                //    textView.setVisibility(View.VISIBLE);
            }
        });
    }
}
