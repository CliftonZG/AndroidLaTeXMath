package io.nano.tex.res;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by nano on 18-11-10
 */
public final class ResManager {

    private static final String TAG = "ResManager";

    private String rootDir;
    private String texDir;

    private Context context;

    public ResManager(Context context) {
        this.context = context;
        rootDir = context.getFilesDir().getPath() + File.separator;
        texDir = rootDir + "io/nano/tex/res";
        Log.v("ResManager", "texDir: " + texDir);
        File directory = new File(texDir);
        if (!directory.exists()) {
            Log.v("ResManager", "texDir doesn't exists, creating one now...");
            directory.mkdirs();  // Create the directory if it doesn't exist
            Log.v("ResManager", "Copying RES_README to texDir...");
            /*
            try {
                copyFileFromAssets(context, "RES_README", texDir, "RES_README");
            } catch (Exception e) {
                Log.e("ResManager.java", e.getMessage());
                if (directory.delete()) Log.w("ResManager.java", "deleted texDir.");
            }

             */

        } else {
            Log.v("ResManager", "texDir already exists.");
        }
        Log.v("ResManager", "texDir: " + texDir);
    }

    public String getResourcesRootDirectory() {
        return "io/nano/tex/res";
    }

    public String[] getFileNames() {
        File texDirectory = new File(texDir);
        if (texDirectory.exists() && texDirectory.isDirectory()) {
            // List all files in the directory
            String[] texFileNames = texDirectory.list();
            if (texFileNames != null) {
                // Log or return the file names
                for (String fileName : texFileNames) {
                    Log.v("ResManager.java", "File: " + fileName);
                }
            } else {
                Log.v("ResManager.java", "The directory is empty or an error occurred.");
            }
        } else {
            Log.v("ResManager.java", "Directory does not exist or is not a directory.");
        }

        File directory = new File(rootDir);
        if (directory.exists() && directory.isDirectory()) {
            // List all files in the directory
            String[] fileNames = directory.list();
            if (fileNames != null) {
                // Log or return the file names
                for (String fileName : fileNames) {
                    Log.v("ResManager.java", "File: " + fileName);
                }
                return fileNames;
            } else {
                Log.v("ResManager.java", "The directory is empty or an error occurred.");
                return new String[]{};
            }
        } else {
            Log.v("ResManager.java", "Directory does not exist or is not a directory.");
            return new String[]{};
        }
    }

    // Utility method to copy a file from assets to internal storage
    public void copyFileFromAssets(Context context, String assetFileName, String destinationDir, String destinationFileName) {
        File directory = new File(destinationDir);
        if (!directory.exists()) {
            Log.i("ResManager.java", directory + ": doesn't exist, creating it now");
            directory.mkdirs(); // Create the directory if it doesn't exist
        } else {
            Log.i("ResManager.java", directory + ": already exists.");
        }

        InputStream inputStream = null;
        OutputStream outputStream = null;

        try {
            // Open the asset as an InputStream
            inputStream = context.getAssets().open(assetFileName);

            // Create the destination file in internal storage
            File destinationFile = new File(destinationDir + File.separator + destinationFileName);
            outputStream = new FileOutputStream(destinationFile);

            // Copy the file from assets to internal storage
            byte[] buffer = new byte[1024];
            int length;
            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }
            Log.v("ResManager", "File copied to: " + destinationFile.getPath());
        } catch (IOException e) {
            Log.e("ResManager", "Error copying file: " + e.getMessage());
        } finally {
            try {
                if (inputStream != null) {
                    inputStream.close();
                }
                if (outputStream != null) {
                    outputStream.close();
                }
            } catch (IOException e) {
                Log.e("ResManager", "Error closing streams: " + e.getMessage());
            }
        }
    }

    private List<String> listRes() throws IOException {
        // InputStream is = ResManager.class.getResourceAsStream("RES_README");
        AssetManager assetManager = context.getAssets();
        InputStream is = assetManager.open("io/nano/tex/res/RES_README");
        Log.v("ResManager", "InputStream from RES_README: " + is);
        BufferedReader in = new BufferedReader(new InputStreamReader(is));
        String line;
        List<String> res = new ArrayList<>();
        try {
            while ((line = in.readLine()) != null) res.add(line);
        } catch (IOException e) {
            Log.e(TAG, "Failed to read resources", e);
        } finally {
            try {
                in.close();
            } catch (IOException e) {
                Log.e(TAG, "Failed to close resources file.", e);
            }
        }
        return res;
    }

    public void checkResources() throws IOException {
        Log.v("ResManager", "checking resources...");
        List<String> res = listRes();
        Log.v("ResManager", "res: " + res);
        File tex = new File(texDir);
        if (!tex.exists()) {
            Log.v("ResManager", "Tex Resource Directory does not exist, will proceed to create a new directory");
            tex.mkdirs();
        } else {
            Log.v("ResManager", "Tex Resource Directory already exists");
        }

        for (String path : res) {
            File file = new File("io/nano/tex/res" + File.separator + path);
            boolean resourceExists = isResourceAvailable(context, String.valueOf(file));
            if (resourceExists) Log.d("ResManager.java", "Resource file is available for: " + file);
            else Log.e("ResManager.java", "Resource file is NOT available for: " + file);
            /*
            File targetFile = new File(texDir + File.separator + path);
            Log.v("ResManager", targetFile.getPath() + ": searching for this file");
            if (targetFile.exists()) {
                Log.v("ResManager", targetFile.getPath() + "already exists");
                continue;  // Skip if the file already exists
            }
             */

            /*
            // Ensure the parent directory exists before creating the file
            int li = path.lastIndexOf(File.separator);
            if (li >= 0) {
                String dir = path.substring(0, li);
                File f = new File(texDir + File.separator + dir);
                if (!f.exists()) f.mkdirs();
            }

            String sourceFile = rootDir + path;
            try {
                // Now copy the resource file from assets to the appropriate location
                //InputStream is = new FileInputStream(sourceFile);
                Log.i(TAG, "Copy resource: " + path);
                //copyTo(is, texDir + File.separator + path);
                copyFileFromAssets(context, path, texDir, path);
            } catch (Exception e) {
                Log.e(TAG, "Error copying resource: " + sourceFile, e);
            }

             */
        }
    }

    private static void copyTo(InputStream is, String targetPath) {
        OutputStream os = null;
        try {
            byte[] buffer = new byte[2048];
            os = new BufferedOutputStream(new FileOutputStream(targetPath));
            int l;
            while ((l = is.read(buffer)) > 0) os.write(buffer, 0, l);
            os.flush();
        } catch (Exception e) {
            Log.e(TAG, "Copy resource failed", e);
        } finally {
            try {
                is.close();
                if (os != null) os.close();
            } catch (IOException e) {
                Log.e(TAG, "Copy resource failed", e);
            }
        }
    }

    public boolean isResourceAvailable(Context context, String filePath) {
        AssetManager assetManager = context.getAssets();
        try {
            // Attempt to open the file with the given path
            assetManager.open(filePath);
            return true; // File exists
        } catch (IOException e) {
            // File does not exist
            return false;
        }
    }
}
