package com.example.android.clientapp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class ChatActivity extends AppCompatActivity {

    private String amigoUserID;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        Intent i = getIntent();
        amigoUserID = (String) i.getExtras().get("userID");

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences
                (getApplicationContext());
        String userID = sharedPref.getString("userID", "");

        SharedPreferences chatsPref = getApplicationContext().getSharedPreferences("chats_user_"+ userID, MODE_PRIVATE);

        int chatSize = chatsPref.getInt("chatSize", 0);
        String chatID = chatsPref.getString("chatID_"+ amigoUserID, null);

        TextView tvNombre = (TextView) findViewById(R.id.tvTitulo);
        tvNombre.setText(String.valueOf(chatSize));

        if (chatID != null) {
            TextView tvNombre2 = (TextView) findViewById(R.id.tvID);
            tvNombre2.setText(chatID);
        }
    }
}
