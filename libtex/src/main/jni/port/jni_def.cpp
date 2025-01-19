#include "jni_def.h"
#include "jni_log.h"

JavaVM* gJVM;

/** io/nano/tex/Rect */
jfieldID gFieldRectX, gFieldRectY, gFieldRectW, gFieldRectH;

/** io/nano/tex/TextLayout */
jclass gClassTextLayout;
jmethodID gMethodGetBounds;

/** io/nano/tex/Font */
jclass gClassFont;
jmethodID gMethodDeriveFont;
jmethodID gMethodCreateFontFromName;
jmethodID gMethodCreateFontFromFile;

/** io/nano/tex/ActionRecorder */
jmethodID gMethodRecord;

jclass gClassAssetJNIManager;
jmethodID gMethodReadFileFromAssets;
jmethodID gMethodGetBytes;

JNIEnv* getJNIEnv() {
    try {
        LOGI("jni_def.cpp, getJNIEnv().");
        JNIEnv* env = 0;
        gJVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        return env;
    } catch (...) {
        LOGE("jni_def.cpp, some unknown error occured.");
    }
}

static void load(JNIEnv* env) {
    try {
        // io/nano/tex/Rect
        LOGI("jni_def.cpp, load(JNIEnv* env).");
        jclass rect = env->FindClass("io/nano/tex/Rect");
        if (!rect) {
            LOGE("Failed to find class io/nano/tex/Rect");
            return;
        }
        gFieldRectX = env->GetFieldID(rect, "x", "F");
        gFieldRectY = env->GetFieldID(rect, "y", "F");
        gFieldRectW = env->GetFieldID(rect, "w", "F");
        gFieldRectH = env->GetFieldID(rect, "h", "F");
        if (!gFieldRectX || !gFieldRectY || !gFieldRectW || !gFieldRectH) {
            LOGE("Failed to get field IDs for io/nano/tex/Rect");
            return;
        }

        // io/nano/tex/TextLayout
        jclass layout = env->FindClass("io/nano/tex/TextLayout");
        if (!layout) {
            LOGE("Failed to find class io/nano/tex/TextLayout");
            return;
        }
        gMethodGetBounds = env->GetStaticMethodID(
                layout,
                "getBounds",
                "(Ljava/lang/String;Lio/nano/tex/Font;)Lio/nano/tex/Rect;");
        if (!gMethodGetBounds) {
            LOGE("Failed to get method ID for getBounds");
            return;
        }
        gClassTextLayout = (jclass)env->NewGlobalRef(layout);

        // io/nano/tex/Font
        jclass font = env->FindClass("io/nano/tex/Font");
        if (!font) {
            LOGE("Failed to find class io/nano/tex/Font");
            return;
        }
        gClassFont = (jclass)env->NewGlobalRef(font);
        gMethodDeriveFont = env->GetMethodID(
                gClassFont,
                "deriveFont",
                "(I)Lio/nano/tex/Font;");
        if (!gMethodDeriveFont) {
            LOGE("Failed to get method ID for deriveFont");
            return;
        }
        gMethodCreateFontFromName = env->GetStaticMethodID(
                gClassFont,
                "create",
                "(Ljava/lang/String;IF)Lio/nano/tex/Font;");
        if (!gMethodCreateFontFromName) {
            LOGE("Failed to get method ID for create (from name)");
            return;
        }
        gMethodCreateFontFromFile = env->GetStaticMethodID(
                gClassFont,
                "create",
                "(Ljava/lang/String;F)Lio/nano/tex/Font;");
        if (!gMethodCreateFontFromFile) {
            LOGE("Failed to get method ID for create (from file)");
            return;
        }

        // io/nano/tex/ActionRecorder
        jclass recorder = env->FindClass("io/nano/tex/ActionRecorder");
        if (!recorder) {
            LOGE("Failed to find class io/nano/tex/ActionRecorder");
            return;
        }
        gMethodRecord = env->GetMethodID(
                recorder,
                "record",
                "(ILjava/lang/Object;[F)V");
        if (!gMethodRecord) {
            LOGE("Failed to get method ID for record");
            return;
        }

        // AssetManager
        jclass assetJNIManager = env->FindClass("io/nano/tex/AssetJNIManager");
        if (!assetJNIManager) {
            LOGE("Failed to find class io/nano/tex/AssetJNIManager");
            return;
        }
        gClassAssetJNIManager = (jclass)env->NewGlobalRef(assetJNIManager);
        gMethodReadFileFromAssets = env->GetStaticMethodID(
                assetJNIManager,
                "readFileFromAssets",
                "(Ljava/lang/String;)Lio/nano/tex/AssetJNIManager;");
        if (!gMethodReadFileFromAssets) {
            LOGE("Failed to get method ID for assetManager");
            return;
        }
        gMethodGetBytes = env->GetMethodID(
                assetJNIManager,
                "getBytes",
                "()[B");
        if (!gMethodGetBytes) {
            LOGE("Failed to get method ID for assetManager");
            return;
        }
    } catch (...) {
        LOGE("jni_def.cpp, some unknown error occurred.");
    }
}

extern int registerLaTeX(JNIEnv* env);
extern int registerTeXRender(JNIEnv* env);

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    try {
        gJVM = vm;

        LOGI("Load jni, initialize java methods...");

        JNIEnv* env = 0;
        vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

        registerLaTeX(env);
        registerTeXRender(env);

        load(env);

        return JNI_VERSION_1_6;
    } catch (...) {
        LOGE("jni_def.cpp, some unknown error occured.");
    }
}
