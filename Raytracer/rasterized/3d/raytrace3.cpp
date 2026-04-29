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

class coordinate{
    public:
    float x;
    float y;
    float z;
};

class triangle{
    public:
    coordinate one;
    coordinate two;
    coordinate three;
    int color;
};

double pi = 3.14159265358979323846;
std::string line;
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
float height;
float facing;
float sensitivity;
float theta;
int direction;

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

std::istream& operator>>(std::istream& is, object& obj){
    return is >> obj.type >> obj.x >> obj.y >> obj.size >> obj.angle >> obj.color;
};

void importobjects(std::string file){
    std::ifstream read(file);
    std::string sep, name;
    object current;
    while (read >> sep >> name >> current){
        shapes[name] = current;
    };
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

std::list<std::list<std::list<float>>> calculate(float xpo, float ypo, float zpo, float looking){
    std::list<std::list<float>> colors;
    for (const auto& [name, shape] : shapes){
        
        for(int i = -height/2; i < he ight/2; i++){
            theta = i/range * fov;
        }
    };
    for(float j = -range/2; j <range/2; j++){
        colors.push_back({0,0});
        angle = j/range * fov + looking;
        stren = 1;
        dist = 1;
        count = 0;
        float xpos = xpo;
        float ypos = ypo;
        float zpos = zpo;
}

int main(){
    importobjects("object.txt");
    fov = pi/2;
    speed = .5;
    x = -10;
    y = 10;
    facing = -pi/6;
    range = 60;
    sensitivity = pi/16;
    height = 16;
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
        auto pixels = calculate(x, y, z, facing);
        for (int i = -height/2; i < height/2; i++){
            display(pixels[i], i);
        };
        refresh();
        napms(1);
        };
    };
};