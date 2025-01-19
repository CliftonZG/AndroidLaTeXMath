#include "jni_def.h"
#include "jni_help.h"
#include "jni_log.h"
#include "latex.h"

#include <ctype.h>
#include <string>

using namespace std;
using namespace tex;

static jboolean LaTeX_init(JNIEnv* env, jclass clazz, jstring resDir) {
    try {
        // Get the UTF-8 C-style string from Java string (resDir)
        const char* rootDir = env->GetStringUTFChars(resDir, NULL);
        // Log the original Java string (rootDir)
        LOGI("Resources root dir: %s, initialize now...", rootDir);
        // Initialize LaTeX with the modified directory
        LaTeX::init(rootDir);
        // Release the Java string memory
        env->ReleaseStringUTFChars(resDir, rootDir);
        LOGI("Initialized successfully.");
        return true;
    } catch (ex_tex& e) {
        LOGE("Failed to initialize the LaTeX engine, caused by: %s", e.what());
        return false;
    } catch (const std::exception& e) {
        LOGE("General exception: %s", e.what());
        return false;
    } catch (...) {
        LOGE("Unknown error occurred during LaTeX initialization.");
        return false;
    }
}

static void LaTeX_release(JNIEnv* env, jclass clazz) {
    LOGI("Release LaTeX engine...");
    LaTeX::release();
}

static jlong LaTeX_parse(
    JNIEnv* env, jclass clazz,
    jstring ltx, jint width, jfloat textSize, jfloat lineSpace, jint foreground) {
    LOGI("jni_latex.cpp, LaTeX_parse");
    try {
        wstring value;
        // Step 1: Convert jstring to std::wstring (UTF-16) for C++ usage
        const jchar* raw = env->GetStringChars(ltx, NULL);
        size_t len = env->GetStringLength(ltx);
        value.assign(raw, raw + len);  // Copy characters to wstring
        // Step 2: Convert jstring to const char* (UTF-8) for logging purposes
        const char* str = env->GetStringUTFChars(ltx, NULL);
        LOGI("jni_latex.cpp, Parse: %s", str);  // Log the LaTeX string
        //env->ReleaseStringUTFChars(ltx, str);  // Release the UTF-8 string
        // Step 3: Call LaTeX::parse and check for null pointer
        TeXRender* r = LaTeX::parse(value, width, textSize, lineSpace, foreground);
        LOGI("jni_latex.cpp, TeXRender* r = LaTeX::parse");
        env->ReleaseStringChars(ltx, raw);  // Don't forget to release the string
        LOGI("jni_latex.cpp, ReleaseStringUTFChars(ltx, str);");
        // Step 4: Return a valid pointer or handle the null pointer case
        if (r != nullptr) {
            LOGI("jni_latex.cpp, reinterpret_cast<long>(r)");
            return reinterpret_cast<long>(r);  // Safe to cast and return
        } else {
            // Handle the null pointer case, e.g., return some error code or default value
            LOGE("jni_latex.cpp, error: reinterpret_cast<long>(r) is nullptr");
            return 0;  // Return 0 to signify a null pointer
        }
    } catch (exception& e) {
        // Catch any exceptions thrown during parsing
        LOGE("jni_latex.cpp, Failed to parse LaTeX, caused by: %s", e.what());
        return 0; // Return 0 in case of an error
    } catch (...) {
        // Catch any exceptions thrown during parsing
        LOGE("jni_latex.cpp, Failed to parse LaTeX, caused by unknown exception");
        return 0; // Return 0 in case of an error
    }
}

static void LaTeX_setDebug(JNIEnv* env, jclass clazz, jboolean debug) {
    bool b = static_cast<bool>(debug);
    LaTeX::setDebug(b);
}

static JNINativeMethod sMethods[] = {
    {"nInit", "(Ljava/lang/String;)Z", (void*)LaTeX_init},
    {"nFree", "()V", (void*)LaTeX_release},
    {"nParse", "(Ljava/lang/String;IFFI)J", (void*)LaTeX_parse},
    {"nSetDebug", "(Z)V", (void*)LaTeX_setDebug}};

int registerLaTeX(JNIEnv* env) {
    return jniRegisterNativeMethods(
        env, "io/nano/tex/LaTeX", sMethods, NELEM(sMethods));
}
