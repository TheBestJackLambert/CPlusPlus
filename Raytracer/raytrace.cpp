#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cmath>
#include <optional>
#include <map>
#include <list>
#include <cstdlib>

class object{
    public:
    std::string type;
    float x;
    float y;
    float size;
    float angle;
    int color;
    
};

class collision{
    public:
    bool coll;
    float angle;
};

class rgb{
    public:
    int r;
    int g;
    int b;
};

constexpr double pi = 3.14159265358979323846;
std::string line;
object nobject;
std::map <std::string, object> shapes;
std::list<std::list<float>> colors;
float fov = pi/3;
float speed = .5;
float xpos;
float ypos;
float angle;
float sinner;
float cosser;
float stren;
float oldx;
float oldy;
float dist;
float count;
collision check;

std::istream& operator>>(std::istream& is, object& obj){
    return is >> obj.type >> obj.x >> obj.y >> obj.size >> obj.angle >> obj.color;
};

rgb hsl_to_rgb(float h, float s, float l){
    rgb output;
    float c = (1 - std::abs(2*l - 1)) * s;
    float x = c * (1 - std::abs(fmod(h/60, 2) - 1));
    float m = l - c/2;
    float rf, gf, bf;
    if      (h < 60)  { rf=c; gf=x; bf=0; }
    else if (h < 120) { rf=x; gf=c; bf=0; }
    else if (h < 180) { rf=0; gf=c; bf=x; }
    else if (h < 240) { rf=0; gf=x; bf=c; }
    else if (h < 300) { rf=x; gf=0; bf=c; }
    else               { rf=c; gf=0; bf=x; }
    output.r = (rf+m)*255;
    output.g = (gf+m)*255;
    output.b = (bf+m)*255;
    return output;
};

void importobjects(std::string file){
    std::ifstream read(file);
    std::string sep, name;
    object current;
    while (read >> sep >> name >> current){
        shapes[name] = current;
    };
};

collision checker(float x, float y, object shape){
    collision result;
    result.coll = false;
    result.angle = shape.angle;
    if (shape.type == "circle"){
        float dist = std::pow(x-shape.x, 2) + std::pow(y-shape.y, 2);
        if( dist <= shape.size * shape.size){
            result.coll = true;
            result.angle = std::atan2((y-shape.y), (x-shape.x));
        };
        return result;
    };
    if (shape.type == "square"){
        float newx = x;
        float newy = y;
        if( shape.angle != 0){
            newx = shape.x + (x - shape.x)*std::cos(-shape.angle) - (y - shape.y)*std::sin(-shape.angle);
            newy = shape.y + (x - shape.x)*std::sin(-shape.angle) + (y - shape.y)*std::cos(-shape.angle);
        };
        if ((shape.size > std::abs(shape.x - newx)) && (shape.size > std::abs(shape.y - newy))){
            result.coll = true;
            if (std::abs(shape.y - newy) > std::abs(shape.x - newx)){
                if (0 < (shape.y - newy)){
                    result.angle += pi/2;
                }
                else{
                    result.angle -= pi/2;
                };
            }   
            else{
                if (0 > (shape.x - newx)){
                    result.angle += pi;
                };
            };
        };
    };
    return result;
};
int main(){
    importobjects("objects.txt");
    for(float j = -10; j < 10; j++){
        colors.push_back({0,0});
        angle = j/20 * fov;
        xpos = -10;
        ypos = 0;
        stren = 1;
        dist = 1;
        count = 0;
        for(int t = 0; t < 100; t++){
            dist *= .95;
            sinner = std::sin(angle);
            cosser = std::cos(angle);
            oldx = xpos;
            oldy = ypos;
            xpos += cosser * speed;
            ypos += sinner * speed;
            for (auto [name, shape] : shapes){
                check = checker(xpos, ypos, shape);
                if (check.coll){
                    colors.back().front() += stren * shape.color;
                    colors.back().back() += dist * stren/2;
                    count += stren;
                    stren *= .5;
                    angle = 2 * check.angle- angle + pi;
                    xpos = oldx;
                    ypos = oldy;
                };
            };
        };
        if (count != 0){
            colors.back().front() /= count;
        };
        float hue = fmod(colors.back().front(), 360.0f);
        while (hue < 0){
            hue += 360;
        }
        colors.back().front() = hue;
    };
    for (const auto& i: colors){
        rgb rbg = hsl_to_rgb(i.front(), 1, i.back());
        printf("\033[48;2;%d;%d;%dm \033[0m", rbg.r, rbg.g, rbg.b);
    };
};