#include <iostream>
#include "configuration.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

using namespace std;


/*
template <typename t>
inline int sgn(t val) {
    return (t(0) < val) - (val < t(0));
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


// this is explanation of how triangle test really works. Version below is just z coordinates of cross products
template<class t>
bool sameSide(const Vec2<t> &a, const Vec2<t> &b, const Vec2<t> & t1, const Vec2<t> & t2)
{
    const Vec2<t> orig = b-a;
    const Vec3<t> cp1 = Vec3<t>::cross(orig, t1-a);
    const Vec3<t> cp2 = Vec3<t>::cross(orig, t2-a);
    return cp1*cp2 >= (t)0;
}

template<class t>
bool triangleTest(const Vec2<t> &p, const Vec2<t> &a, const Vec2<t> & b, const Vec2<t> & c)
{
    return sameSide(a, b, c, p)
            && sameSide(a, c, b, p)
            && sameSide(c, b, a, p);
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

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);


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


template<class t>
bool triangleTest(const Vec2<t> &p, const Vec2<t> &a, const Vec2<t> & b, const Vec2<t> & c)
{
    //same as before, just simplified a little
    const auto z1 = (b.y-a.y)*(p.x-a.x) - (b.x-a.x)*(p.y-a.y);
    const auto z2 = (c.y-b.y)*(p.x-b.x) - (c.x-b.x)*(p.y-b.y);
    const auto z3 = (a.y-c.y)*(p.x-c.x) - (a.x-c.x)*(p.y-c.y);
    return  (z1 >= 0 && z2 >=0 && z3>=0) || (z1 <= 0 && z2 <=0 && z3<=0);
}

void triangle(Vec2i a, Vec2i b, Vec2i c, TGAImage &image, TGAColor col) {

    int x0 = min(a.x,min(b.x,c.x));
    int y0 = min(a.y,min(b.y,c.y));

    int x1 = max(a.x,max(b.x,c.x));
    int y1 = max(a.y,max(b.y,c.y));

    for (int x = x0; x <= x1 ; x++) {
        for (int y = y0; y <= y1 ; y++) {
            Vec2i p = Vec2i(x,y);
            if(triangleTest(p, a, b, c)) {
                image.set(x, y, col);
            }
        }
    }

}


void flatShadedModel(Model * model, TGAImage &image)
{
    Vec3f light_dir = Vec3f(0,0,-1);
    int width = image.get_width();
    int height = image.get_height();
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }
}

int main() {
    TGAImage image(800, 800, TGAImage::RGB);

    Model model(RES("head.obj"));

    //RenderWireframe(model, image, white);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    flatShadedModel(&model, image);

    image.flip_vertically();

    image.write_tga_file(OUT("output.tga"));

    return 0;
}
