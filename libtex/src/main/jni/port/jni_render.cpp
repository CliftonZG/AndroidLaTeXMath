#include "config.h"

#ifdef __OS_Android__

#include "graphics_android.h"
#include "jni_def.h"
#include "jni_help.h"
#include "jni_log.h"

#include "latex.h"

using namespace tex;

static void TeXRender_draw(
    JNIEnv* env, jclass clazz,
    jlong ptr, jobject recorder, jint x, jint y) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        LOGI("Draw: %p, in position (%d, %d), with recorder: %p", r, x, y, recorder);
        Graphics2D_Android g2(recorder);
        r->draw(g2, x, y);
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }

}

static jfloat TeXRender_getTextSize(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        return r->getTextSize();
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }

}

static jint TeXRender_getHeight(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        return r->getHeight();
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }

}

static jint TeXRender_getDepth(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        return r->getDepth();
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static jint TeXRender_getWidth(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        return r->getWidth();
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static jfloat TeXRender_getBaseline(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        return r->getBaseline();
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static void TeXRender_setTextSize(JNIEnv* env, jclass clazz, jlong ptr, jfloat size) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        r->setTextSize((float)size);
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static void TeXRender_setForeground(JNIEnv* env, jclass clazz, jlong ptr, jint c) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        r->setForeground((color)c);
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static void TeXRender_setWidth(JNIEnv* env, jclass clazz, jlong ptr, jint width, jint align) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
    r->setWidth(width, align);
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static void TeXRender_setHeight(JNIEnv* env, jclass clazz, jlong ptr, jint height, jint align) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        r->setHeight(height, align);
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

static void TeXRender_finalize(JNIEnv* env, jclass clazz, jlong ptr) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        TeXRender* r = reinterpret_cast<TeXRender*>(ptr);
        LOGI("TeXRender finalize: %p", r);
    delete r;
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }

}

static JNINativeMethod sMethods[] = {
    {"nDraw", "(JLio/nano/tex/ActionRecorder;II)V", (void*)TeXRender_draw},
    {"nGetTextSize", "(J)F", (void*)TeXRender_getTextSize},
    {"nGetHeight", "(J)I", (void*)TeXRender_getHeight},
    {"nGetDepth", "(J)I", (void*)TeXRender_getDepth},
    {"nGetWidth", "(J)I", (void*)TeXRender_getWidth},
    {"nGetBaseline", "(J)F", (void*)TeXRender_getBaseline},
    {"nSetTextSize", "(JF)V", (void*)TeXRender_setTextSize},
    {"nSetForeground", "(JI)V", (void*)TeXRender_setForeground},
    {"nSetWidth", "(JII)V", (void*)TeXRender_setWidth},
    {"nSetHeight", "(JII)V", (void*)TeXRender_setHeight},
    {"nFinalize", "(J)V", (void*)TeXRender_finalize}};

int registerTeXRender(JNIEnv* env) {
    try {
        LOGI("jni_render.cpp, TeXRender* LaTeX::parse");
        return jniRegisterNativeMethods(
        env, "io/nano/tex/TeXRender", sMethods, NELEM(sMethods));
    } catch (const std::exception& e) {
        LOGE("jni_render.cpp, %s", e.what());
        throw;
    } catch (...) {
        LOGE("jni_render.cpp, unknown exception");
        throw;
    }
}

#endif  // __OS_Android__
