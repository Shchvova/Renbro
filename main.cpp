#include <iostream>
#include "configuration.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


void line_obvious(Vec2i a, Vec2i b, TGAImage & image, const TGAColor & col) {
    const auto d = b-a;
    const int dx = abs(d.x);
    const int dy = abs(d.y);

    if(dx > dy) {
        if(a.x > b.x)
            swap(a,b);

        int s;
        if(dx>0)
            s = sgn(b.y-a.y);
        else
            s = 0;

        for(int x=0; x<= dx; x++) {
            image.set(a.x + x, a.y + x*s*dy/dx, col);
        }
    }else if (dy>0)
    {

        if(a.y > b.y)
            swap(a,b);

        int s;
        if(dy>0)
            s = sgn(b.x-a.x);
        else
            s = 0;

        for(int y=0; y<= dy; y++) {
            image.set(a.x + y*s*dx/dy, a.y+y, col);
        }
    }
    else
    {
        for(int x=0; x<= dx; x++) {
            image.set(a.x + x, a.y, col);
        }
    }
}


void line(Vec2i a, Vec2i b, TGAImage &image, const TGAColor &col) {
    bool swap = false;
    if (abs(a.x-b.x)<abs(a.y-b.y)) {
        a.swap();
        b.swap();
        swap = true;
    }
    if (a.x>b.x) {
        std::swap(a, b);
    }
    Vec2i d = b-a;
    int dError = abs(d.y)*2;
    int error = 0;
    int y = a.y;
    for (int x=a.x; x<=b.x; x++) {
        if (swap) {
            image.set(y, x, col);
        } else {
            image.set(x, y, col);
        }
        error += dError;

        if (error > d.x) {
            y += (b.y>a.y?1:-1);
            error -= d.x*2;
        }
    }
}


const TGAColor white = TGAColor(255, 255, 255, 255);

int main() {
    int width = 800, height = 800;
    TGAImage image(width, height, TGAImage::RGB);

    Model model(RES("head.obj"));

    for (int i=0; i<model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j+1)%3]);
            int x0 = int((v0.x+1.0)*width*0.5);
            int y0 = int((v0.y+1.0)*height*0.5);
            int x1 = int((v1.x+1.0)*width*0.5);
            int y1 = int((v1.y+1.0)*height*0.5);
            line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
            //line(x0, y0, x1, y1, image2, white);
        }
    }



    image.flip_vertically();

    image.write_tga_file(OUT("output.tga"));

    return 0;
}
