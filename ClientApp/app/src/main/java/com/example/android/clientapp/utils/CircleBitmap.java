package com.example.android.clientapp.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Shader;

/**
 * Created by emanuel on 11/22/16.
 */

public class CircleBitmap {
    public static Bitmap generate(Bitmap bitmap) {
        Bitmap circleBitmap = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
        BitmapShader shader =   new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.CLAMP);
        Paint paint = new Paint();
        paint.setShader(shader);
        Canvas canvas = new Canvas(circleBitmap);
        canvas.drawCircle(bitmap.getWidth()/2, bitmap.getHeight()/2, bitmap.getWidth()/2, paint);
        return circleBitmap;
    }

    public static Bitmap resize_thumbnail(Bitmap bitmap, int destWidth, int destHeight) {
        Bitmap new_bitmap = Bitmap.createScaledBitmap(bitmap, destWidth, destHeight, false);
        return new_bitmap;
    }
}
