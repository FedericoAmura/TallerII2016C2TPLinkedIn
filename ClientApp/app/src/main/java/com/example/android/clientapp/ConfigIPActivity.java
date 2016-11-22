package com.example.android.clientapp;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;
import android.os.AsyncTask;

import java.io.IOException;
import java.net.InetAddress;

public class ConfigIPActivity extends AppCompatActivity {

    private EditText n1;
    private EditText n2;
    private EditText n3;
    private EditText n4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_config_ip);

        n1 = (EditText) findViewById(R.id.num1);
        n2 = (EditText) findViewById(R.id.num2);
        n3 = (EditText) findViewById(R.id.num3);
        n4 = (EditText) findViewById(R.id.num4);
        String[] ip = JobifyAPI.getIP().split("\\.");
        n1.setText(ip[0]);
        n2.setText(ip[1]);
        n3.setText(ip[2]);
        n4.setText(ip[3]);

        Button boton = (Button) findViewById(R.id.botonOK);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                apretarOK(v);
            }
        });
    }

    private void apretarOK(View view) {
        String resIP = n1.getText().toString().trim()+"."+n2.getText().toString().trim()+
                "."+n3.getText().toString().trim()+"."+n4.getText().toString().trim();
        new PingTask().execute(resIP);
    }

    private class PingTask extends AsyncTask<String, Void, Boolean> {

        protected String ip;

        @Override
        protected Boolean doInBackground(String... strings) {
            this.ip = strings[0];
            try
            {
                Runtime runtime = Runtime.getRuntime();
                Process  proc = runtime.exec("/system/bin/ping -c 4 "+ip);
                int retVal = proc.waitFor();
                if(retVal==0) return true;
                else return false;
            }
            catch (IOException | InterruptedException e)
            {
                return false;
            }
        }

        @Override
        protected void onPreExecute() {
            ProgressBar progressBar = (ProgressBar) findViewById(R.id.pingLoading);
            progressBar.setVisibility(View.VISIBLE);
        }

        @Override
        protected void onPostExecute(Boolean canConnect) {
            ProgressBar progressBar = (ProgressBar) findViewById(R.id.pingLoading);
            progressBar.setVisibility(View.INVISIBLE);
            if (!canConnect)  Toast.makeText(ConfigIPActivity.this,"IP unreachable",Toast.LENGTH_LONG).show();
            else {
                Toast.makeText(ConfigIPActivity.this,"IP OK",Toast.LENGTH_LONG).show();
                JobifyAPI.setIP(ip);
                finish();
            }
        }
    }
}


