package io.nano.tex.test;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by nano on 18-11-13
 */
public class Samples {

    private Context context;
    private int index = 0;
    private List<String> samples = new ArrayList<>();

    public Samples(Context context) {
        this.context = context;
    }

    public void readSamples() {
        Log.v("Samples.java", "readSamples");
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(context.getAssets().open("SAMPLES.tex")))) {
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = reader.readLine()) != null) {
                //Log.v("Samples.java", "line: " + line);
                if (!line.isEmpty() && !isSpace(line) && isSeparator(line)) {
                    //Log.v("Samples.java", "readSamples, addSample, isSeperator: " + sb.toString());
                    addSample(sb.toString());
                    sb.delete(0, sb.length());
                    // sb.setLength(0);  // Clear the StringBuilder
                } else if (!line.isEmpty() && !isSpace(line)) {
                    sb.append(line).append('\n');
                    //Log.v("Samples.java", "readSamples, addSample, append(line), sb: " + sb.toString());
                }
                //Log.v("Samples.java", "samples: " + samples);
            }
        } catch (IOException e) {
            Log.e("Samples.java", "Error reading sample file: " + e.getMessage(), e);
        } catch (Exception e) {
            Log.e("Samples.java", e.getMessage());
        }
    }

    private boolean isSpace(String str) {
        try {
            //Log.v("Samples.java", "isSpace: " + space);
            return str.trim().isEmpty();
        } catch (Exception e) {
            Log.e("Samples.java", e.getMessage());
            throw new RuntimeException("Samples.java");
        }
    }

    private boolean isSeparator(String str) {
        try {
            for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) != '%') {
                //Log.v("Samples.java", "isSeparator: false");
                return false;
            }
        }
        //Log.v("Samples.java", "isSeparator: true");
        return true;
        } catch (Exception e) {
            Log.e("Samples.java", e.getMessage());
            throw new RuntimeException("Samples.java");
        }
    }

    private void addSample(String sample) {
        try {
            if (sample != null && !sample.trim().isEmpty()) {
                samples.add(sample);
            }
        } catch (Exception e) {
            Log.e("Samples.java", "synchronized TeXRender parse error: " + e.getMessage());
            throw new RuntimeException("Samples.java");
        }

    }

    public String next() {
        try {
            if (samples.isEmpty()) {
                Log.w("Samples.java", "No samples available");
                return null;
            }

            if (index >= samples.size()) {
                index = 0;  // Reset index to loop through the samples
            }

            String sample = samples.get(index);
            index++;
            return sample;
        } catch (Exception e) {
            Log.e("Samples.java", "synchronized TeXRender parse error: " + e.getMessage());
            return null; // Gracefully return null if something goes wrong
        }

    }

    public String current() {
        try {
            if (samples.isEmpty()) {
                Log.w("Samples.java", "No samples available");
                return null;
            }

            String sample = samples.get(index);
            return sample;
        } catch (Exception e) {
            Log.e("Samples.java", "synchronized TeXRender parse error: " + e.getMessage());
            return null; // Gracefully return null if something goes wrong
        }

    }
}
