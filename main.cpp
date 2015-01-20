#include <iostream>
#include "configuration.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


void line(Vec2i a, Vec2i b, TGAImage & image, const TGAColor & col) {
    const auto d = b-a;
    const int dx = abs(d.x);
    const int dy = abs(d.y);
    if(dy > dx) {

    }
}


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}


const TGAColor white = TGAColor(255, 255, 255, 255);

int main() {
    int width = 800, height = 800;
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage image2(width, height, TGAImage::RGB);


    Model model(RES("head.obj"));

    for (int i=0; i<model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j+1)%3]);
            int x0 = (v0.x+1.0)*width/2.0;
            int y0 = (v0.y+1.0)*height/2.0;
            int x1 = (v1.x+1.0)*width/2.0;
            int y1 = (v1.y+1.0)*height/2.0;
            line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
            line(x0, y0, x1, y1, image2, white);
        }
    }


//    line(Vec2i(100, 5), Vec2i(300, 700), image, white);
//    line(100, 5, 300, 700, image2, white);
//
//    line(Vec2i(300, 700),Vec2i(100, 5),  image, white);
//    line(300, 700, 100, 5,  image2, white);

    image.flip_vertically();
    image2.flip_vertically();

    image.write_tga_file(OUT("output.tga"));
    image2.write_tga_file(OUT("output2.tga"));

    return 0;
}
