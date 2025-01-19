package io.nano.tex.test;

import android.os.Bundle;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import io.nano.tex.LaTeX;

/**
 * Created by nano on 18-11-12
 */
public class MainActivity extends AppCompatActivity {

    private TeXView texView;
    private Samples samples;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try {
            samples = new Samples(this);
            Log.v("MainActivity", "created instance of sample class");
        } catch (Exception e) {
            Log.e("MainActivity", "unable to create instance of sample class: " + e.getMessage());
        }
        try {
            samples.readSamples();
            Log.v("MainActivity", "samples loaded");
        } catch (Exception e) {
            Log.e("MainActivity", "unable to read samples: " + e.getMessage());
        }
        texView = findViewById(R.id.tex);
        //String latex = samples.current();
        //texView.setLaTeX(latex);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        try {
            getMenuInflater().inflate(R.menu.main, menu);
            return super.onCreateOptionsMenu(menu);
        } catch (Exception e) {
            Log.e("MainActivity", "synchronized TeXRender parse error: " + e.getMessage());
            return false;
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        try {
            if (item.getItemId() == R.id.next_sample) {
                try {
                    nextSample();
                } catch (Exception e) {
                    Log.e("MainActivity", "onOptionsItemSelected, nextSample: " + e.getMessage());
                    return false;
                }
                return true;
            } else if (item.getItemId() == R.id.debug) {
                switchDebug(item);
                return true;
            } else {
                return super.onOptionsItemSelected(item);
            }
        } catch (Exception e) {
            Log.e("MainActivity", "Error handling menu item: " + e.getMessage(), e);
            return false; // Avoid crashing, return false so the item is not marked as selected
        }
    }

    private void nextSample() {
        try {
            if (texView != null) {
                String latex = samples.next();
                Log.v("MainActivity", "nextSample, latex: " + latex);
                if (latex != null) {
                    // Make sure the UI update happens on the main thread
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Log.v("MainActivity", "setLatex");
                            texView.setLaTeX(latex);
                        }
                    });
                } else {
                    Log.e("MainActivity", "LaTeX content is null.");
                }
            } else {
                Log.e("MainActivity", "texView is not initialized.");
            }
        } catch (Exception e) {
            Log.e("MainActivity", "Error while setting LaTeX: " + e.getMessage(), e);
            throw new RuntimeException("MainActivity", e);
        }
    }

    private void switchDebug(MenuItem item) {
        try {
            boolean isDebug = LaTeX.instance().isDebug();
            LaTeX.instance().setDebug(!isDebug);
            item.setChecked(LaTeX.instance().isDebug());
            if (texView != null) {
                texView.invalidateRender();
            } else {
                Log.e("MainActivity", "texView is not initialized.");
            }
        } catch (Exception e) {
            Log.e("MainActivity", "Error toggling debug mode: " + e.getMessage(), e);
        }
    }
}
