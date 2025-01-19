package io.nano.tex;

import android.content.Context;
import android.graphics.Typeface;
import android.util.Log;

/**
 * Created by nano on 18-11-10
 */
public final class Font {

    private Typeface typeface;
    private float size;

    private Font(Typeface typeface, float size) {
        this.typeface = typeface;
        Log.v("Font.java", "typeface = "+ this.typeface);
        this.size = size;
        Log.v("Font.java", "size = "+ this.size);
    }

    public Font deriveFont(int style) {
        if (typeface.getStyle() == style) return this;
        Typeface typeface = Typeface.create(this.typeface, style);
        return new Font(typeface, size);
    }

    public Typeface getTypeface() {
        return typeface;
    }

    public float getSize() {
        return size;
    }

    public static Font create(String name, int style, float size) {
        try {
            Typeface typeface = Typeface.create(name, style);
            return new Font(typeface, size);
        } catch (Exception e) {
            Log.e("Font.java, create(String name, int style, float size), ", "error:" + e);
            throw new RuntimeException("cannot create font");
        }

    }

    public static Font create(String file, float size) {
        Log.v("Font.java", "create(String file, float size), Typeface file: " + file);
        try {
            Context context = LaTeX.instance().getContext();
            if (context == null) {
                Log.v("Font.java", "Context is null");
                throw new RuntimeException("cannot create font");
            } {
                Typeface tf = Typeface.createFromAsset(context.getAssets(), file);
                Log.v("Font.java", "Font create(String file, float size), Typeface: " + tf.toString());
                return new Font(tf, size);
            }
            //return null;
        } catch (Exception e) {
            Log.e("Font.java", "create(String file, float size), error:" + e);
            throw new RuntimeException("cannot create font");
        }
    }
}
