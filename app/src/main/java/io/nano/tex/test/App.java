package io.nano.tex.test;

import android.app.Application;
import android.util.Log;

import java.security.spec.ECField;

import io.nano.tex.LaTeX;

/**
 * Created by nano on 18-11-12
 */
public class App extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        try {
            LaTeX latexInstance = LaTeX.instance();
            Log.v("nanotextest", "latexInstance: " + latexInstance);
            if (latexInstance != null) {
                latexInstance.init(this);
            }
        } catch (Exception e) {
            Log.e("nanotextest", String.valueOf(e));
        }
    }
}
