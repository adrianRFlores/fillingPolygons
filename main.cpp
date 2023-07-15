#include "Color.h"
#include "Vertex.h"
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <array>
#define screenWidth 800
#define screenHeight 800

Color clearColor(0,0,0);
Color borderColor(255,255,255);
Color currentColor(100,100,100);
std::array<Color, screenWidth*screenHeight> framebuffer;
long int count = 0;

void clear() {
	framebuffer.fill(clearColor);
}

void setCurrentColor(const unsigned char r, const unsigned char g, const unsigned char b) {
	currentColor = Color(r, g, b);
}

void setBorderColor(const unsigned char r, const unsigned char g, const unsigned char b) {
	borderColor = Color(r, g, b);
}

void point(const Vertex2& vert, const bool border = false) {
	if(vert.y < screenHeight && vert.x < screenWidth){
		framebuffer[vert.y * screenWidth + vert.x] = border ? borderColor : currentColor;
	}
}

void renderBuffer() {

	//std::cout<<"write";

	unsigned char header[54] = {
        0x42, 0x4D,             // Signature: "BM"
        0x36, 0x4C, 0x1D, 0x00, // File size in bytes (54 + image size, assuming no padding)
        0x00, 0x00,             // Reserved
        0x00, 0x00,             // Reserved
        0x36, 0x00, 0x00, 0x00, // Offset to pixel data (54 bytes)
        0x28, 0x00, 0x00, 0x00, // Size of the header (40 bytes)
        0x20, 0x03, 0x00, 0x00, // Width of the image (800 pixels)
        0x20, 0x03, 0x00, 0x00, // Height of the image (800 pixels)
        0x01, 0x00,             // Number of color planes (1)
        0x18, 0x00,             // Bits per pixel (24-bit color)
        0x00, 0x00, 0x00, 0x00, // Compression method (none)
        0x00, 0x4C, 0x1D, 0x00, // Image size in bytes (size of pixel data, assuming no padding)
        0x00, 0x00, 0x00, 0x00, // Horizontal resolution (not specified)
        0x00, 0x00, 0x00, 0x00, // Vertical resolution (not specified)
        0x00, 0x00, 0x00, 0x00, // Number of colors in the color palette (none)
        0x00, 0x00, 0x00, 0x00  // Number of important colors (all)
    };

    FILE* f = fopen("out.bmp", "wb");

    if(f != nullptr) {
    	fwrite(header, sizeof(unsigned char), sizeof(header), f);
    	for(int i = 0; i < screenWidth * screenHeight; i++) {
    		unsigned char buf[] = {framebuffer[i].b, framebuffer[i].g, framebuffer[i].r};
    		fwrite(buf, sizeof(unsigned char), sizeof(buf), f);
    	}
    }

    fclose(f);

}

void clampVertex(Vertex2& p){
	//No se revisa que sea menor a 0 porque se supone que es un unsigned int
	if(p.x > screenWidth) p.x = screenWidth;
	if(p.y > screenHeight) p.y = screenHeight;
}

void line(Vertex2& p1, Vertex2& p2){
	clampVertex(p1);
	clampVertex(p2);
	Vertex2 x0(p1.x, p1.y);
	int dx = abs(p2.x - p1.x);
	int sx = p1.x < p2.x ? 1 : -1;
	int dy = -abs(p2.y - p1.y);
	int sy = p1.y < p2.y ? 1 : -1;
	int error = dx + dy;

	while(true) {
		point(x0, true);
		if(x0.x == p2.x && x0.y == p2.y) break;
		int e2 = 2 * error;
		if(e2 >= dy) {
			if(x0.x == p2.x) break;
			error += dy;
			x0 = Vertex2(x0.x + sx, x0.y);
		}
		if(e2 <= dx) {
			if(x0.y == p2.y) break;
			error += dx;
			x0 = Vertex2(x0.x, x0.y + sy);
		}
	}
}

void drawPolygon(Vertex2 arr[], unsigned int arrSize) {
	if(arrSize > 1){
		for(unsigned int i = 0; i<arrSize-1; i++) {
			line(arr[i], arr[i+1]);
		}
		line(arr[arrSize-1], arr[0]);
	}
	
}

bool isInBounds(const Vertex2& p) {
	if(p.x > 0 && p.x < screenWidth) {
		if(p.y > 0 && p.y < screenWidth) {
			return true;
		}
	}
	return false;
}

bool isInBounds(const unsigned int x, const unsigned int y) {
	if(x > 0 && x < screenWidth) {
		if(y > 0 && y < screenWidth) {
			return true;
		}
	}
	return false;
}

bool compareColor(const Color& c1, const Color& c2) {
	if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b) return true;
	return false;
}

void fillPolygon(const unsigned int x, const unsigned int y) {

	if(isInBounds(x, y)) {
		if(compareColor(framebuffer[y * screenWidth + x], borderColor)){
			return;
		}
		else if(compareColor(framebuffer[y * screenWidth + x], currentColor)){
			return;
		}

		point(Vertex2(x, y));

		fillPolygon(x, y - 1);
		fillPolygon(x, y + 1);
		fillPolygon(x + 1, y);
		fillPolygon(x - 1, y);
	}

	
}

void render() {
	clear();
	//Vertex2 verts[3] = {Vertex2(377,249), Vertex2(411,197), Vertex2(436,249)};
	//Vertex2 verts[4] = {Vertex2(100,100), Vertex2(150, 100), Vertex2(150,150), Vertex2(100, 150)};
	//Vertex2 verts[4] = {Vertex2(682,175), Vertex2(708,120), Vertex2(735,148), Vertex2(739,170)};
	Vertex2 poly4[18] = {Vertex2(413, 177), Vertex2(448, 159), Vertex2(502, 88), Vertex2(553, 53), Vertex2(535, 36), Vertex2(676, 37), Vertex2(660, 52), Vertex2(750, 145), Vertex2(761, 179), Vertex2(672, 192), Vertex2(659, 214), Vertex2(615, 214), Vertex2(632, 230), Vertex2(580, 230), Vertex2(597, 215), Vertex2(552, 214), Vertex2(517, 144), Vertex2(466, 180)};
	Vertex2 poly5[4] = {Vertex2(682,175), Vertex2(708,120), Vertex2(735,148), Vertex2(739,170)};
	//Vertex2 verts[10] = {Vertex2(165,380), Vertex2(185,360), Vertex2(180,330), Vertex2(207,345), Vertex2(233,330), Vertex2(230, 360), Vertex2(250,380), Vertex2(220,385), Vertex2(205,410), Vertex2(193,383)};
	drawPolygon(poly4, sizeof(poly4)/sizeof(poly4[0]));
	setCurrentColor(100,100,100);
	fillPolygon(707, 148);

	drawPolygon(poly5, sizeof(poly5)/sizeof(poly5[0]));
	setCurrentColor(0,0,0);
	fillPolygon(707,148);
	//fillPolygon(200, 350);

	renderBuffer();
}



int main(int argc, char* argv[]){
	render();
}