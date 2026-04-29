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
#include <vector>

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
};

class line{
    public:
    coordinate one;
    coordinate two;
    int color;
};

class endpoint{
    public:
    coordinate one;
    coordinate two;
    float startangle;
    float endangle;
    float sdist;
    float fdist;
    float color;

    float dist(float theta) const{
        float distance;
        float height = sqrt(sdist * fdist) * sin(endangle - startangle) / (sqrt(sdist+ fdist - 2 * sqrt(sdist * fdist) * cos(endangle - startangle)));
        distance = height / cos(endangle - acos(height/sqrt(fdist)) - theta);
        return distance;
    };
};

double pi = 3.14159265358979323846;
std::string lin;
std::vector<line>lines;
std::vector<std::vector<line>> layers;
std::map<int, int> colorCache;
int nextColorId = 9;
float fov;
float speed;
float x;
float y;
float angle1;
float angle2;
float dist;
float sinner;
float cosser;
float stren;
float dist1;
float dist2;
float range;
float facing;
float sensitivity;
int height = 16;
int direction;

std::istream& operator>>(std::istream& is, coordinate& obj){
    return is >> obj.x >> obj.y;
};

std::istream& operator>>(std::istream& is, line& obj){
    return is >> obj.one >> obj.two >> obj.color;
};

void importobjects(std::string file){
    std::ifstream read(file);
    std::string token;
    while (read >> token){
        if (token == "|"){
            layers.push_back(lines);
            lines.clear();
            continue;
        }
        line current;
        current.one.x = std::stof(token);
        read >> current.one.y >> current.two.x >> current.two.y >> current.color;
        lines.push_back(current);
    }
    if (!lines.empty()){
        layers.push_back(lines);
    }
}

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

void display(std::list<std::list<float>> color, int row){
    int col = 0;
    for (const auto& i : color){
        col++;
        rgb rbg = hsl_to_rgb(i.front(), 1, i.back());
        int key = (rbg.r/8 << 16) | (rbg.g/8 << 8) | (rbg.b/8);
        int cid;
        if (colorCache.count(key)){
            cid = colorCache[key];
        }
        else{
            cid = nextColorId++;
            init_color(cid, rbg.r * 1000/255, rbg.g * 1000/255, rbg.b * 1000/255);
            init_pair(cid, COLOR_WHITE, cid);
            colorCache[key] = cid;
        }
        attron(COLOR_PAIR(cid));
        mvaddch(row, col, ' ');
        attroff(COLOR_PAIR(cid));
    }
}

std::list<std::list<float>> calculate(float xpo, float ypo, float looking, int i){
    std::vector<line> linelist = layers[i];
    std::list<std::list<float>> colors;
    std::map<float, endpoint> startpoints;
    for (const auto& current : linelist){
        endpoint point;
        point.one.x = current.one.x;
        point.two.x = current.two.x;
        point.one.y = current.one.y;
        point.two.y = current.two.y;
        point.startangle = atan2((current.one.y - ypo), (current.one.x - xpo));
        point.endangle = atan2((current.two.y - ypo), (current.two.x - xpo));
        point.sdist = std::pow(current.one.x - xpo, 2) + std::pow(current.one.y - ypo, 2);
        point.fdist = std::pow(current.two.x - xpo, 2) + std::pow(current.two.y - ypo, 2);
        point.color = current.color;
        if (point.endangle < point.startangle){
            point.endangle += 2 * pi;
        };
        if (point.endangle - point.startangle > pi){
            std::swap(point.startangle, point.endangle);
            point.endangle += 2 * pi;
        };
        startpoints[point.startangle] = point;
    };
    float theta;
    endpoint best;
    float currentdist;
    float bestdist;
    bool check;
    for (int i = -range/2; i < range/2; i++){
        check = 1;
        theta = i/range * fov + looking;
        float adj = theta;
        for(const auto& [start, current] : startpoints){
            adj = theta + (theta < start ? 2*pi : 0);
            if(start <= adj and adj <= current.endangle){
                currentdist = current.dist(adj);
                if (check){
                    best = current;
                    bestdist = currentdist;
                    check = 0;
                }
                else{
                    if (currentdist < bestdist){
                        best = current;
                        bestdist = currentdist;
                    };
                }
            }
        };
        if (check){
            colors.push_back({0,0});
        }
        else{
            float gilbert = std::pow(.975, bestdist * 10);
            colors.push_back({best.color, gilbert});
        };
    };
    return colors;
};

int main(){
    importobjects("lines.txt");
    endwin();
    fov = pi/2;
    speed = .5;
    x = 0;
    y = 1.5;
    facing = 0;
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
                    if (facing >  pi) facing -= 2*pi;
                    if (facing < -pi) facing += 2*pi;
                }
            }
        colorCache.clear();
        nextColorId = 9;
        for (int j = 0; j < layers.size(); j++){
            auto pixels = calculate(x, y, facing, j);
            display(pixels, j);
        };
        init_color(COLOR_WHITE, 1000, 1000, 1000);
        attron(A_BOLD);
        addstr(("\n x: " + std::to_string(x) + "\n").c_str());
        addstr(("y: " + std::to_string(y) + "\n").c_str());
        addstr(("\n Angle: " + std::to_string(facing * 180 / pi) + "\n").c_str());
        refresh();
        napms(1);
        };
    };
};