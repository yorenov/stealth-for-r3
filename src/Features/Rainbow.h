#pragma once
#include <imgui.h>

struct Color {
    int r, g, b, a;

    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 255;
    }

    Color(const int r, const int g, const int b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }

    Color(const int r, const int g, const int b, const int a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    static Color FromHSB(const float hue, const float saturation, const float brightness) {
        const float h = hue == 1.0f ? 0 : hue * 6.0f;
        const float f = h - static_cast<int>(h);
        const float p = brightness * (1.0f - saturation);
        const float q = brightness * (1.0f - saturation * f);
        const float t = brightness * (1.0f - (saturation * (1.0f - f)));

        if (h < 1) {
            return Color(
                static_cast<unsigned char>(brightness * 255),
                static_cast<unsigned char>(t * 255),
                static_cast<unsigned char>(p * 255)
            );
        } else if (h < 2) {
            return Color(
                static_cast<unsigned char>(q * 255),
                static_cast<unsigned char>(brightness * 255),
                static_cast<unsigned char>(p * 255)
            );
        } else if (h < 3) {
            return Color(
                static_cast<unsigned char>(p * 255),
                static_cast<unsigned char>(brightness * 255),
                static_cast<unsigned char>(t * 255)
            );
        } else if (h < 4) {
            return Color(
                static_cast<unsigned char>(p * 255),
                static_cast<unsigned char>(q * 255),
                static_cast<unsigned char>(brightness * 255)
            );
        } else if (h < 5) {
            return Color(
                static_cast<unsigned char>(t * 255),
                static_cast<unsigned char>(p * 255),
                static_cast<unsigned char>(brightness * 255)
            );
        } else {
            return Color(
                static_cast<unsigned char>(brightness * 255),
                static_cast<unsigned char>(p * 255),
                static_cast<unsigned char>(q * 255)
            );
        }
    }

    static Color FromImColor(const ImColor &color) {
        return Color(static_cast<int>(color.Value.z * 255), static_cast<int>(color.Value.y * 255),
                     static_cast<int>(color.Value.x * 255), static_cast<int>(color.Value.w * 255));
    }

    static ImColor ToImColor(const Color color) {
        return ImColor(color.b / 255.f, color.g / 255.f, color.r / 255.f, color.a / 255.f);
    }
};

class CRainbow {
public:
    static void Update();
};

extern CRainbow *pRainbow;
