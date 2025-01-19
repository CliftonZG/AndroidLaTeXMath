package io.nano.tex;

import android.content.Context;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

public final class AssetJNIManager {

    private final byte[] bytes;

    private AssetJNIManager(byte[] bytes) {
        this.bytes = bytes;
        Log.v("AssetManager.java", "bytes = "+ Arrays.toString(this.bytes));
    }

    public byte[] getBytes() {
        return this.bytes;
    }
    public static AssetJNIManager readFileFromAssets(String file) throws IOException {
        Log.v("AssetManager.java", "readFileFromAssets(String file), file: " + file);
        try {
            Context context = LaTeX.instance().getContext();
            if (context == null) {
                Log.v("AssetManager.java", "Context is null");
                throw new RuntimeException("cannot get AssetManager");
            } else {
                android.content.res.AssetManager assetManager = context.getAssets();
                InputStream inputStream = assetManager.open(file);
                ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

                byte[] buffer = new byte[1024];
                int length;
                while ((length = inputStream.read(buffer)) > 0) {
                    byteArrayOutputStream.write(buffer, 0, length);
                }

                inputStream.close();
                return new AssetJNIManager(byteArrayOutputStream.toByteArray());
            }
        } catch (Exception e) {
            Log.e("AssetManager.java", "readFileFromAssets(String file), error:" + e);
            throw new RuntimeException("readFileFromAssets(String file)");
        }
    }
}
