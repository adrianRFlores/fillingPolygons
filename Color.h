#pragma once
#include <iostream>

unsigned char sum(unsigned char val1, unsigned char val2) {
    if(val1 + val2 > 255) return 255;
    else return val1 + val2;
}

unsigned char clampMult(float val, unsigned char val2) {
    if(val < 0) return 0;
    else if(val * val2 > 255) return 255;
    else return val * val2;
}

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color(int red = 0, int green = 0, int blue = 0)
        : r(colorClamp(red)), g(colorClamp(green)), b(colorClamp(blue)) {}

    private:
        //Clamp para evitar valores invalidos
        unsigned char colorClamp(const int value) {
            if (value > 255) return 255;
            else if (value < 0) return 0;
            else return value;
        }      

};

//Overload de << para imprimir los valores del color.
std::ostream& operator<<(std::ostream& os, const Color& color) {
    os<<"r"<<(int)color.r<<" g"<<(int)color.g<<" b"<<(int)color.b;
    return os;
}

//Suma de dos colores
Color sumColors(const Color& color1, const Color& color2) {
    return Color(sum(color1.r, color2.r), sum(color1.g, color2.g), sum(color1.b, color2.b));
}

//Escalar * color
Color mult(const float val, const Color& color) {
    return Color(clampMult(val, color.r), clampMult(val, color.g), clampMult(val, color.b));
}