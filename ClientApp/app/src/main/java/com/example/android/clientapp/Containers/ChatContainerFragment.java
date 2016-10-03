package com.example.android.clientapp.Containers;

/**
 * Created by guidonegri on 21/09/16.
 */

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.android.clientapp.Fragments.ChatFragment;
import com.example.android.clientapp.R;

public class ChatContainerFragment extends BaseContainerFragment {

    private boolean mIsViewInited;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        Log.d("TEST", "Tab CHAT oncreateview");
        return inflater.inflate(R.layout.fragment_base_container, null);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        Log.d("TEST", "Tab CHAT container on activity created");
        if (!mIsViewInited) {
            mIsViewInited = true;
            initView();
        }
    }

    private void initView() {
        Log.d("TEST", "Tab CHAT init view");
        replaceFragment(new ChatFragment(), false);
    }

}