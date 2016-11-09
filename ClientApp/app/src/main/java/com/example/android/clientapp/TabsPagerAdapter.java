package com.example.android.clientapp;

/**
 * Created by guidonegri on 01/10/16.
 */

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.util.Log;

import com.example.android.clientapp.Containers.BusquedaContainerFragment;
import com.example.android.clientapp.Containers.ChatContainerFragment;
import com.example.android.clientapp.Containers.PerfilContainerFragment;
import com.example.android.clientapp.Containers.RelacionesContainerFragment;
import com.example.android.clientapp.Fragments.BusquedaFragment;
import com.example.android.clientapp.Fragments.ChatFragment;
import com.example.android.clientapp.Fragments.PerfilFragment;
import com.example.android.clientapp.Fragments.RelacionesFragment;

public class TabsPagerAdapter extends FragmentPagerAdapter {

    private boolean f0;
    private boolean f1;
    private boolean f2;
    private boolean f3;

    public TabsPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int index) {
        Log.d("TEST", "ITEM INDEX: " + index);
        switch (index) {
            case 0:
                Log.d("TEST", "New PerfilFragment");
                return new PerfilFragment();
            case 1:
                Log.d("TEST", "New RelacionesFragment");
                return new RelacionesFragment();
            case 2:
                Log.d("TEST", "New BusquedaFragment");
                return new BusquedaFragment();
            case 3:
                Log.d("TEST", "New ChatFragment");
                return new ChatFragment();
        }
        return null;
    }

    @Override
    public int getCount() {
        // get item count - equal to number of tabs
        return 4;
    }
}