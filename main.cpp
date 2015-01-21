#include <iostream>
#include "configuration.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

using namespace std;


/*
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//rendering line with most apparent algorithm is not that effective, but good to double check
//other implementations. Also, not so pretty as well, due to rounding issues.
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
*/

//using (https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
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


void RenderWireframe(const Model & model, TGAImage & image, const TGAColor & col ) {
    int width = image.get_width();
    int height = image.get_height();
    for (int i=0; i<model.nfaces(); i++) {
        auto face = model.face(i);
        for (int j=0; j<3; j++) {
            Vec3f a = model.vert(face[j]);
            Vec3f b = model.vert(face[(j+1)%3]);

            int xa = int((a.x+1.0)*width*0.5);
            int ya = int((a.y+1.0)*height*0.5);
            int xb = int((b.x+1.0)*width*0.5);
            int yb = int((b.y+1.0)*height*0.5);
            line(Vec2i(xa, ya), Vec2i(xb, yb), image, col);
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
}


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

int main() {
    TGAImage image(200, 200, TGAImage::RGB);

    Model model(RES("head.obj"));

    //RenderWireframe(model, image, white);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};


    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);


    image.flip_vertically();

    image.write_tga_file(OUT("output.tga"));

    return 0;
}
