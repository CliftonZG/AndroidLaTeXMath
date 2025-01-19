package io.nano.tex;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.IOException;

import io.nano.tex.res.ResManager;

/**
 * Created by nano on 18-11-10
 */
public final class LaTeX {

    private static boolean libLoaded = false;
    private static LaTeX instance;

    private Context appContext;



    /**
     * Get the instance of the LaTeX engine.
     */
    public synchronized static LaTeX instance() {
        if (!libLoaded) {
            try {
                Log.v("LaTeX", "Loading libtex library...");
                System.loadLibrary("tex");
                Log.v("LaTeX", "Successfully Loaded libtex library...");
            } catch (Exception e) {
                Log.e("LaTeX", String.valueOf(e));
            }

            libLoaded = true;
        }
        if (instance == null) {
            Log.v("LaTeX", "No instance of libtex found, creating a new instance...");
            try {
                instance = new LaTeX();
                Log.v("LaTeX", "Successfully created a new instance of libtex!");
                if (instance == null) {
                    Log.e("LaTeX", "New instance of libtex is somehow null!!");
                }
            } catch (Exception e) {
                Log.e("LaTeX", "Could not create new instance of libtex!!!");
            }


        }
        Log.v("LaTeX", "Returning instance...");
        return instance;
    }

    private volatile boolean initialized = false;
    private volatile boolean isDebug = false;

    private LaTeX() {
    }

    /**
     * Initialize the LaTeX engine. Call of this function will copy the "TeX resources" from apk into the
     * data files directory of the host application, and parse the "TeX resources", it may takes long time,
     * you may call it from a background thread.
     */
    public synchronized void init(Context context) throws IOException {
        appContext = context;
        Log.v("LaTeX", "initializing new ResManager from context...");
        ResManager rm = new ResManager(context);
        Log.i("LaTeX", "initialized new ResManager from context...");
        String[] fileNames = rm.getFileNames();
        for (String fileName : fileNames) {
            Log.d("LaTeX", "Found file: " + fileName);
        }
        Log.v("LaTeX", "need to check resources...");
        rm.checkResources();
        boolean success = nInit(rm.getResourcesRootDirectory());
        if (!success) {
            Log.e("LaTeX", "Failed to unpack resources!!!");
            throw new TeXException("Failed to initialize LaTeX engine.");
        }
        initialized = true;
    }

    /**
     * Check if the LaTeX engine is initialized.
     */
    public boolean isInitialized() {
        return initialized;
    }

    /**
     * Release the LaTeX engine.
     */
    public synchronized void release() {
        try {
            nFree();
            initialized = false;
        } catch (Exception e) {
            Log.e("LaTeX.java", "synchronized TeXRender parse error: " + e.getMessage());
            throw new RuntimeException("LaTeX.java\", \"synchronized TeXRender parse error");
        }
    }

    private void check() {
        if (!initialized) throw new IllegalStateException(
            "The engine was not initialized, call init(Context) before use.");
    }

    /**
     * Parse a TeX formatted code with specified text size and foreground color.
     */
    public synchronized TeXRender parse(String ltx, float textSize, int foreground) {
        try {
            check();
            long ptr = nParse(ltx, 0, textSize, 0, foreground);
            if (ptr == 0) throw new TeXException("Failed to parse LaTeX: " + ltx);
            return new TeXRender(ptr);
        } catch (Exception e) {
            Log.e("LaTeX.java", "synchronized TeXRender parse error: " + e.getMessage());
            throw new RuntimeException("LaTeX.java\", \"synchronized TeXRender parse error");
        }
    }

    /**
     * Parse a TeX code
     *
     * @param ltx        the TeX formatted code
     * @param width      the width of the 2D graphics context
     * @param textSize   the text size to draw
     * @param lineSpace  the space between two lines
     * @param foreground the foreground color
     */
    public synchronized TeXRender parse(
        String ltx, int width,
        float textSize, float lineSpace,
        int foreground) {
        check();
        Log.v("LaTeX", ltx);
        Log.v("LaTeX", String.valueOf(width));
        Log.v("LaTeX", String.valueOf(textSize));
        Log.v("LaTeX", String.valueOf(lineSpace));
        Log.v("LaTeX", String.valueOf(foreground));
        try {
            long ptr = nParse(ltx, width, textSize, lineSpace, foreground);
            Log.v("LaTeX","ok");
            if (ptr == 0) throw new TeXException("Failed to parse LaTeX: " + ltx);

            // We got a very long formula, scale to fit the width
            TeXRender r = new TeXRender(ptr);
            if (r.getWidth() > width) {
                float w = r.getWidth();
                float scale = width / w;
                r.setTextSize(scale * textSize);
            }
            return r;

        } catch (Exception e) {
            Log.e("LaTeX", String.valueOf(e));
        }
        return null;
    }

    /**
     * Set if debug
     */
    public synchronized void setDebug(boolean debug) {
        try {
            nSetDebug(debug);
            isDebug = debug;
        } catch (Exception e) {
            Log.e("LaTeX", String.valueOf(e));
        }
    }

    /**
     * Check if is in debug mode.
     */
    public boolean isDebug() {
        return isDebug;
    }

    private static native boolean nInit(String resDir);

    private static native void nFree();

    private static native long nParse(
        String ltx, int width,
        float textSize, float lineSpace,
        int foreground);

    private static native void nSetDebug(boolean debug);

    public Context getContext() {
        return appContext;
    }

    public AssetManager getAssetManager() {
        return appContext.getAssets();
    }
}
