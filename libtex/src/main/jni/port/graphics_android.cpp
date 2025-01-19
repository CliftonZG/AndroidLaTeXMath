#include "config.h"

#ifdef __OS_Android__

#include "jni_def.h"
#include "jni_log.h"

#include "common.h"
#include "graphics_android.h"

#include <cstring>

using namespace tex;
using namespace std;

/******************************************* Android Font *****************************************/

static void _delete_java_ref(jobject ptr) {
    try {
        LOGI("graphics_android.cpp, _delete_java_ref(jobject ptr)");
        JNIEnv* env = getJNIEnv();
        env->DeleteGlobalRef(ptr);
        LOGI("Delete global java ref: %p", ptr);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Font_Android::setSize(float size) {
    try {
        LOGI("graphics_android.cpp, Font_Android::setSize(float size)");
        _size = size;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

void Font_Android::setJavaFont(jobject font) {
    try {
        LOGI("graphics_android.cpp, setJavaFont");
        //LOGI("graphics_android.cpp, %p", font);
        jobject ref = getJNIEnv()->NewGlobalRef(font);
        _javaFont = shared_ptr<_jobject>(ref, _delete_java_ref);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

shared_ptr<_jobject> Font_Android::getJavaFont() const {
    try {
        LOGI("graphics_android.cpp, Font_Android::getJavaFont()");
       return _javaFont;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

float Font_Android::getSize() const {
    try {
        LOGI("graphics_android.cpp, Font_Android::getSize()");
        return _size;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

shared_ptr<Font> Font_Android::deriveFont(int style) const {
    try {
        LOGI("graphics_android.cpp, Font_Android::deriveFont(int style)");
        Font_Android* f = new Font_Android();
        JNIEnv* env = getJNIEnv();
        jobject obj = env->CallObjectMethod(_javaFont.get(), gMethodDeriveFont, style);
        f->_size = _size;
        if (env->IsSameObject(_javaFont.get(), obj)) {
            f->_javaFont = _javaFont;
        } else {
            f->setJavaFont(obj);
        }
        LOGI("Derive font, java font: %p, style: %d, derived java font: %p",
             _javaFont.get(), style, f->_javaFont.get());
        return shared_ptr<Font>(f);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

bool Font_Android::operator==(const Font& f) const {
    try {
        LOGI("graphics_android.cpp, Font_Android::operator==(const Font& f)");
        return _javaFont.get() == static_cast<const Font_Android&>(f)._javaFont.get();
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

bool Font_Android::operator!=(const Font& f) const {
    try {
        LOGI("graphics_android.cpp, Font_Android::operator!=(const Font& f)");
        return !(*this == f);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
        throw;
    }
}

Font* Font::create(const string& file, float size) {
    LOGI("graphics_android.cpp, Font::create(const string& file, float size)");
    try {
        LOGI("graphics_android.cpp, font file: %s", file.c_str());
        Font_Android* f = new Font_Android();
        JNIEnv* env = getJNIEnv();
        LOGI("graphics_android.cpp, gotten JNIENV");
        jstring str = env->NewStringUTF(file.c_str());
        jobject obj = env->CallStaticObjectMethod(
            gClassFont, gMethodCreateFontFromFile, str, size);
        LOGI("graphics_android.cpp, obj");
        f->setSize(size);
        LOGI("graphics_android.cpp, set size to %f", size);
        f->setJavaFont(obj);
        env->DeleteLocalRef(str);
        return f;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

shared_ptr<Font> Font::_create(const string& name, int style, float size) {
    LOGI("graphics_android.cpp, Font::_create(const string& name, int style, float size)");
    try {
        LOGI("graphics_android.cpp, font file: %s", name.c_str());
        Font_Android* f = new Font_Android();
        JNIEnv* env = getJNIEnv();
        LOGI("graphics_android.cpp, gotten JNIENV");
        jstring str = env->NewStringUTF(name.c_str());
        jobject obj = env->CallStaticObjectMethod(
            gClassFont, gMethodCreateFontFromName, str, style, size);
        LOGI("graphics_android.cpp, obj");
        f->setSize(size);
        LOGI("graphics_android.cpp, set size to %f", size);
        f->setJavaFont(obj);
        env->DeleteLocalRef(str);
        return shared_ptr<Font>(f);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

/******************************************* Text layout ******************************************/

void TextLayout_Android::getBounds(_out_ Rect& bounds) {
    try {
        LOGI("graphics_android.cpp, TextLayout_Android::getBounds(_out_ Rect& bounds)");
        JNIEnv* env = getJNIEnv();
        shared_ptr<_jobject> jfont = static_cast<Font_Android*>(_font.get())->getJavaFont();
        jstring str = env->NewStringUTF(wide2utf8(_txt.c_str()).c_str());
        jobject obj = env->CallStaticObjectMethod(
            gClassTextLayout, gMethodGetBounds, str, jfont.get());
        bounds.x = env->GetFloatField(obj, gFieldRectX);
        bounds.y = env->GetFloatField(obj, gFieldRectY);
        bounds.w = env->GetFloatField(obj, gFieldRectW);
        bounds.h = env->GetFloatField(obj, gFieldRectH);

        env->DeleteLocalRef(str);
        env->DeleteLocalRef(obj);

        LOGI("TextLayout::getBounds: [%.2f, %.2f, %.2f, %.2f]", bounds.x, bounds.y, bounds.w, bounds.h);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void TextLayout_Android::draw(Graphics2D& g2, float x, float y) {
    try {
        LOGI("graphics_android.cpp, TextLayout_Android::draw(Graphics2D& g2, float x, float y)");
        const Font* oldFont = g2.getFont();
        g2.setFont(_font.get());
        string str = wide2utf8(_txt.c_str());
        LOGI("TextLayout::draw, text: %s", str.c_str());
        g2.drawText(_txt, x, y);
        g2.setFont(oldFont);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

shared_ptr<TextLayout> TextLayout::create(const wstring& txt, const shared_ptr<Font>& font) {
    try {
        LOGI("graphics_android.cpp, TextLayout::create(const wstring& txt, const shared_ptr<Font>& font)");
        return shared_ptr<TextLayout>(new TextLayout_Android(txt, font));
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

/******************************************* Graphics 2D ******************************************/

/**
 * Keep in sync with io/nano/tex/ActionRecorder
 */
enum Action {
    ACT_setFont,
    ACT_setColor,
    ACT_setStrokeWidth,
    ACT_setStroke,
    ACT_translate,
    ACT_scale,
    ACT_rotate,
    ACT_reset,
    ACT_drawChar,
    ACT_drawText,
    ACT_drawLine,
    ACT_drawRect,
    ACT_fillRect,
    ACT_drawRoundRect,
    ACT_fillRoundRect
};

enum AffineTransformIndex {
    SX,
    SY,
    TX,
    TY,
    R,
    PX,
    PY
};

Graphics2D_Android::Graphics2D_Android(jobject jrecorder) : _font(nullptr), _stroke() {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android(jobject jrecorder)");
        _t = new float[9]();
        _t[SX] = _t[SY] = 1;
        _color = black;
        JNIEnv* env = getJNIEnv();
        _jrecorder = env->NewGlobalRef(jrecorder);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

Graphics2D_Android::~Graphics2D_Android() {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::~Graphics2D_Android()");
        delete _t;
        JNIEnv* env = getJNIEnv();
        env->DeleteGlobalRef(_jrecorder);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::makeRecord(int action, jobject arg, const jfloat* args, int argsLen) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::makeRecord(int action, jobject arg, const jfloat* args, int argsLen)");
        jfloatArray arr = 0;
        JNIEnv* env = getJNIEnv();
        if (argsLen != 0) {
            arr = env->NewFloatArray(argsLen);
            env->SetFloatArrayRegion(arr, 0, argsLen, args);
        }
        env->CallVoidMethod(_jrecorder, gMethodRecord, action, arg, arr);
        if (arr) env->DeleteLocalRef(arr);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::setColor(color c) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::setColor(color c)");

    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    _color = c;
    jfloat p[] = {static_cast<jfloat>((int)c)};
    makeRecord(ACT_setColor, NULL, p, 1);
}

color Graphics2D_Android::getColor() const {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::getColor()");

    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    return _color;
}

void Graphics2D_Android::setStroke(const Stroke& s) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::setStroke(const Stroke& s)");

    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    _stroke = s;
    jfloat p[] = {
        s.lineWidth,
        s.miterLimit,
        static_cast<jfloat>(s.cap),
        static_cast<jfloat>(s.join)};
    makeRecord(ACT_setStroke, NULL, p, 4);
}

const Stroke& Graphics2D_Android::getStroke() const {
    try {
        LOGI("graphics_android.cpp, troke& Graphics2D_Android::getStroke()");
        return _stroke;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::setStrokeWidth(float w) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::setStrokeWidth(float w)");
        _stroke.lineWidth = w;
        jfloat p[] = {w};
    makeRecord(ACT_setStrokeWidth, NULL, p, 1);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

const Font* Graphics2D_Android::getFont() const {
    try {
        LOGI("graphics_android.cpp, Font* Graphics2D_Android::getFont()");
        return _font;
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::setFont(const Font* font) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::setFont(const Font* font)");
        _font = font;
        if (font == nullptr) {
            makeRecord(ACT_setFont, NULL, NULL, 0);
            return;
        }
        const Font_Android* f = static_cast<const Font_Android*>(font);
        makeRecord(ACT_setFont, f->getJavaFont().get(), NULL, 0);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::translate(float dx, float dy) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::translate(float dx, float dy)");
        _t[TX] += _t[SX] * dx;
        _t[TY] += _t[SY] * dy;
        jfloat p[] = {dx, dy};
        makeRecord(ACT_translate, NULL, p, 2);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::scale(float sx, float sy) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::scale(float sx, float sy)");
        _t[SX] *= sx;
        _t[SY] *= sy;
        jfloat p[] = {sx, sy};
        makeRecord(ACT_scale, NULL, p, 2);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::rotate(float angle) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::rotate(float angle)");
        rotate(angle, 0, 0);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::rotate(float angle, float px, float py) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::rotate(float angle, float px, float py)");
        float r = (float)(angle / PI * 180);
        _t[R] += r;
        _t[PX] = px * _t[SX] + _t[TX];
        _t[PY] = py * _t[SY] + _t[TY];
        jfloat p[] = {angle, px, py};
        makeRecord(ACT_rotate, NULL, p, 3);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::reset() {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::reset()");
        memset(_t, 0, sizeof(float) * 9);
        _t[SX] = _t[SY] = 1;
        makeRecord(ACT_reset, NULL, NULL, 0);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

float Graphics2D_Android::sx() const {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::sx()");
        return _t[SX];
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

float Graphics2D_Android::sy() const {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::sy()");
        return _t[SY];
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

void Graphics2D_Android::drawChar(wchar_t c, float x, float y) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::drawChar(wchar_t c, float x, float y)");
        jfloat p[] = {static_cast<jfloat>(c), x, y};
        makeRecord(ACT_drawChar, NULL, p, 3);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
    
}

void Graphics2D_Android::drawText(const wstring& c, float x, float y) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::drawText(const wstring& c, float x, float y)");
        string str = wide2utf8(c.c_str());
        JNIEnv* env = getJNIEnv();
        jstring jstr = env->NewStringUTF(str.c_str());
        jfloat p[] = {x, y};
        makeRecord(ACT_drawText, jstr, p, 2);
        env->DeleteLocalRef(jstr);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

void Graphics2D_Android::drawLine(float x1, float y1, float x2, float y2) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::drawLine(float x1, float y1, float x2, float y2)");
        jfloat p[] = {x1, y1, x2, y2};
        makeRecord(ACT_drawLine, NULL, p, 4);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::drawRect(float x, float y, float w, float h) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::drawRect(float x, float y, float w, float h)");
        jfloat p[] = {x, y, w, h};
        makeRecord(ACT_drawRect, NULL, p, 4);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }
}

void Graphics2D_Android::fillRect(float x, float y, float w, float h) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::fillRect(float x, float y, float w, float h)");
        jfloat p[] = {x, y, w, h};
        makeRecord(ACT_fillRect, NULL, p, 4);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::drawRoundRect(float x, float y, float w, float h, float rx, float ry) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::drawRoundRect(float x, float y, float w, float h, float rx, float ry)");
        jfloat p[] = {x, y, w, h, rx, ry};
        makeRecord(ACT_drawRoundRect, NULL, p, 6);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

void Graphics2D_Android::fillRoundRect(float x, float y, float w, float h, float rx, float ry) {
    try {
        LOGI("graphics_android.cpp, Graphics2D_Android::fillRoundRect(float x, float y, float w, float h, float rx, float ry)");
        jfloat p[] = {x, y, w, h, rx, ry};
        makeRecord(ACT_fillRoundRect, NULL, p, 6);
    } catch (const std::exception& e) {
        LOGE("graphics_android.cpp, %s", e.what());
        throw std::runtime_error("graphics_android.cpp");
    } catch (...) {
        LOGE("graphics_android.cpp, some unknown error occured.");
    }

}

#endif  // __OS_Android__