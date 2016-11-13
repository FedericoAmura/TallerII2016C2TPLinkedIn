package com.example.android.clientapp;

import android.database.DataSetObserver;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.AbsListView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;

import com.example.android.clientapp.ArrayAdapters.MessageArrayAdapter;
import com.example.android.clientapp.Modelo.chat.Message;

public class ChatActivity extends AppCompatActivity {
    private MessageArrayAdapter messageArrayAdapter;
    private ListView msglist;
    private Button buttonSend;
    private EditText editTextSend;
    private String amigoUserID;
    private String nombreAmigo = "?";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        buttonSend = (Button) findViewById(R.id.btn_send);
        editTextSend = (EditText) findViewById(R.id.edit_msg);
        msglist = (ListView) findViewById(R.id.msg_list);

        messageArrayAdapter = new MessageArrayAdapter(getApplicationContext(), R.layout.right);
        msglist.setAdapter(messageArrayAdapter);

        buttonSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!editTextSend.getText().toString().isEmpty())
                    sendMessage();
            }
        });

        Bundle bundle = getIntent().getExtras();
        nombreAmigo = bundle.getString("name");

        setToolbar();

        msglist.setTranscriptMode(AbsListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
        msglist.setAdapter(messageArrayAdapter);

        messageArrayAdapter.registerDataSetObserver(new DataSetObserver() {
            @Override
            public void onChanged() {
                super.onChanged();
                msglist.setSelection(messageArrayAdapter.getCount()-1);
            }
        });

        messageArrayAdapter.add(new Message("Hola", false));
        messageArrayAdapter.add(new Message("Hola1", false));
        messageArrayAdapter.add(new Message("Hola2", false));
        messageArrayAdapter.add(new Message("Hola4", true));
        messageArrayAdapter.add(new Message("Hola", false));
        messageArrayAdapter.add(new Message("Hola1", true));
        messageArrayAdapter.add(new Message("Hola2", false));
        messageArrayAdapter.add(new Message("Hola4", true));
    }

    private void setToolbar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null)
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle(nombreAmigo);
    }

    private void sendMessage() {
        messageArrayAdapter.add(new Message(editTextSend.getText().toString(), true));
        editTextSend.setText("");
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case android.R.id.home:
                super.onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

}
