package io.nano.tex.test;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import androidx.annotation.Nullable;

import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import io.nano.tex.Graphics2D;
import io.nano.tex.LaTeX;
import io.nano.tex.TeXRender;

/**
 * Created by nano on 18-11-12
 */
public class TeXView extends View {

    public TeXView(Context context) {
        super(context);
    }

    public TeXView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public TeXView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public TeXView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    private int textSize = 40;
    private TeXRender render;
    private Graphics2D g2 = new Graphics2D();

    public void setLaTeX(String ltx) {
        try {
            int w = getWidth();
            if (w == 0) w = 2048;
            Log.v("TeXView.java", "setLaTeX, parsing the current latex");
            render = LaTeX.instance().parse(ltx, w, textSize, 10, Color.DKGRAY);
            Log.v("TeXView.java", "setLaTeX, render and parse complete");
            requestLayout();
            Log.v("TeXView.java", "setLaTeX, requestLayout complete");
        } catch (Exception e) {
            Log.e("TeXView.java", "setLaTeX: " + e.getMessage());
        }

    }

    public void setTextSize(int size) {
        try {
            Log.v("TeXView.java", "setTextSize(int "+ size + ")");
            textSize = size;
            if (render != null) render.setTextSize(size);
            requestLayout();
        } catch (Exception e) {
            Log.e("TeXView.java", "setTextSize error: " + e.getMessage());
        }
    }

    public void invalidateRender() {
        try {
            if (render != null) {
                render.invalidateDrawingCache();
            } else {
                Log.v("TeXView.java", "invalidateRender, TeXRender is null, skipping...");
            }
            invalidate();
        } catch (Exception e) {
            Log.e("TeXView.java", "invalidateRender error: " + e.getMessage());
        }

    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        try {
            Log.v("TeXView.java", "onMeasure(int widthMeasureSpec, int heightMeasureSpec)");
            super.onMeasure(widthMeasureSpec, heightMeasureSpec);
            if (render == null) return;
            int h = render.getHeight();
            setMeasuredDimension(getMeasuredWidth(), h + getPaddingTop() + getPaddingBottom());
        } catch (Exception e) {
            Log.e("TeXView.java", "onMeasure error: " + e.getMessage());
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        try {
            Log.v("TeXView.java", "onDraw(Canvas canvas)");
            if (render == null) return;
            g2.setCanvas(canvas);
            render.draw(g2, getPaddingLeft(), getPaddingTop());
        } catch (Exception e) {
            Log.e("TeXView.java", "onDraw error: " + e.getMessage());
        }
    }
}
