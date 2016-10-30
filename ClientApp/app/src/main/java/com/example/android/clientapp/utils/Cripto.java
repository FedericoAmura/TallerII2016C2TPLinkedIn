package com.example.android.clientapp.utils;

import android.util.Base64;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Created by guidonegri on 29/10/16.
 */

public class Cripto {

    public static String encodeString(String str) {
        try {
            String hash = null;
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(str.getBytes("UTF-8"));
            byte[] shaDig = md.digest();
            hash = Base64.encodeToString(shaDig, Base64.DEFAULT);
            return hash;
        }
        catch (NoSuchAlgorithmException e) {e.printStackTrace();}
        catch (UnsupportedEncodingException e) {e.printStackTrace();}
        return null;
    }
}
