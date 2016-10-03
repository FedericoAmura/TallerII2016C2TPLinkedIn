package com.example.android.clientapp;

/**
 * Created by guidonegri on 01/10/16.
 */

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import com.example.android.clientapp.Fragments.BusquedaFragment;
import com.example.android.clientapp.Fragments.ChatFragment;
import com.example.android.clientapp.Fragments.PerfilFragment;
import com.example.android.clientapp.Fragments.RelacionesFragment;

public class TabsPagerAdapter extends FragmentPagerAdapter {

    public TabsPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int index) {

        Fragment fragment;
        switch (index) {
            case 0:
                fragment = new PerfilFragment();
                return fragment;
            case 1:
                fragment = new RelacionesFragment();
                return fragment;
            case 2:
                fragment = new BusquedaFragment();
                return fragment;
            case 3:
                fragment = new ChatFragment();
                return fragment;
        }
        return null;
    }

    @Override
    public int getCount() {
        // get item count - equal to number of tabs
        return 4;
    }



}