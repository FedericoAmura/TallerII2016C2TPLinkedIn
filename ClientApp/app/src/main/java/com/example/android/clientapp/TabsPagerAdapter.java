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

    public TabsPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int index) {
        Log.d("TEST", "ITEM INDEX: " + index);
        switch (index) {
            case 0:
                return new PerfilContainerFragment();
            case 1:
                return new RelacionesContainerFragment();
            case 2:
                return new BusquedaContainerFragment();
            case 3:
                return new ChatContainerFragment();
        }
        return null;
    }

    @Override
    public int getCount() {
        // get item count - equal to number of tabs
        return 4;
    }
}

/*public class TabsPagerAdapter extends FragmentStatePagerAdapter {

    //integer to count number of tabs
    int tabCount;

    //Constructor to the class
    public TabsPagerAdapter(FragmentManager fm, int tabCount) {
        super(fm);
        //Initializing tab count
        this.tabCount= tabCount;
    }

    //Overriding method getItem
    @Override
    public Fragment getItem(int position) {
        //Returning the current tabs
        switch (position) {
            case 0:
                return new PerfilContainerFragment();
            case 1:
                return new RelacionesContainerFragment();
            case 2:
                return new BusquedaContainerFragment();
            case 3:
                return new ChatContainerFragment();
            default:
                return null;
        }
    }

    //Overriden method getCount to get the number of tabs
    @Override
    public int getCount() {
        return tabCount;
    }
}*/