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
#include <ncurses.h>

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
    short r;
    short g;
    short b;
};

double pi = 3.14159265358979323846;
std::string line;
object nobject;
std::map <std::string, object> shapes;
float fov;
float speed;
float x;
float y;
float angle;
float sinner;
float cosser;
float stren;
float oldx;
float oldy;
float dist;
float count;
float range;
float facing;
float sensitivity;
int direction;
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

void display(std::list<std::list<float>> color, int row){
    int counter = 0;
    for (const auto& i: color){
        counter++;
        rgb rbg = hsl_to_rgb(i.front(), 1, i.back());
        init_color(counter, rbg.r * 1000/255, rbg.g * 1000/255, rbg.b* 1000/255);
        init_pair(counter, COLOR_WHITE, counter);
        attron(COLOR_PAIR(counter));
        mvaddch(row, counter, ' ');
    };
};

std::list<std::list<float>> calculate(float xpo, float ypo, float looking){
    std::list<std::list<float>> colors;
    for(float j = -range/2; j <range/2; j++){
        colors.push_back({0,0});
        angle = j/range * fov + looking;
        stren = 1;
        dist = 1;
        count = 0;
        float xpos = xpo;
        float ypos = ypo;
        for(int t = 0; t < 1000; t++){
            dist *= .975;
            sinner = std::sin(angle);
            cosser = std::cos(angle);
            oldx = xpos;
            oldy = ypos;
            xpos += cosser * speed;
            ypos += sinner * speed;
            for (const auto& [name, shape] : shapes){
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
    return colors;
};

int main(){
    importobjects("objects.txt");
    fov = pi/2;
    speed = .5;
    x = -10;
    y = 10;
    facing = -pi/6;
    range = 60;
    sensitivity = pi/16;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nodelay(stdscr, true);
    start_color();
    while (true){
        direction = getch();
        if (direction != ERR){
            clear();
            if (direction == 'q'){
                break;
            }
            else{
                if(direction == 'w' | direction == 's' | direction == 'a' | direction == 'd'){
                    if(direction == 'w' | direction == 's'){
                        if (direction == 'w'){
                            y += sin(facing);
                            x += cos(facing);
                        }
                        else{
                            y -= sin(facing);
                            x -= cos(facing);
                        }
                    }
                    else{
                        if (direction == 'a'){
                            y -= cos(facing);
                            x += sin(facing);
                        }
                        else{
                            y += cos(facing);
                            x -= sin(facing);
                        }
                    }
                }
                else{
                    if (direction == KEY_LEFT){
                        facing -= sensitivity;
                    }
                    else{
                        if (direction == KEY_RIGHT){
                            facing += sensitivity;
                        }
                    }
                }
            }
            auto pixels = calculate(x, y, facing);
        for (int i = 0; i < 15; i++){
            display(pixels, i);
        };
        refresh();
        napms(1);
        };
    };
};