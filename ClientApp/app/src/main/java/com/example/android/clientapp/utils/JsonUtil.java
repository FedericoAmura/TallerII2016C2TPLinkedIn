package com.example.android.clientapp.utils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.ArrayList;

public final class JsonUtil {

    /**
     * Dado un json con un campo que tiene un array de strings, devuelve el array de strings
     * @param json                   Json a desmenuzar
     * @param fieldName             Nombre del campo con el array
     * @return                      Un arraylist con los valores
     */
    public static ArrayList<String> jsonToStringArray(JSONObject json, String fieldName) {
        ArrayList<String> result = new ArrayList<String>();
        try {
            JSONArray jArray = json.getJSONArray(fieldName);
            for (int i = 0; i < jArray.length(); ++i)
                result.add(jArray.getString(i));
        }
        catch(JSONException e) {
            e.printStackTrace();
        }
        return result;
    }
}
