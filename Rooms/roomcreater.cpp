#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <fstream>

bool john = true;
int count = 0;

struct grid{
    std::string name;
    int x;
    int y;
};

std::vector<grid> points = {};

int main() {
    std::string name;
    int x;
    int y;

    while (john){
        std:: cin >> name;
        if (name == "null") {
            john = false;
        }
        else{
            std:: cin >> x;
            std:: cin >> y;
            grid robert = grid{name, x, y};
            points.push_back(robert);
            count++;
        };
    };
    std::ofstream outFile("rooms.txt");
    for(int i = 0; i < count; i++){
        outFile << "|" << std::endl;
        outFile << points[i].name << std::endl;
        std::string rooms[4] = {"null", "null", "null", "null"};
        for(int j = 0; j < count; j++){
            if (points[i].y == points[j].y){
            if (points[j].x + 1 == points[i].x){
                rooms[3] = points[j].name;
            }
            else{
                if (points[j].x - 1 == points[i].x){
                    rooms[1] = points[j].name;
                };
                };
            };
            if (points[i].x == points[j].x){
            if (points[j].y + 1 == points[i].y){
                rooms[0] = points[j].name;
            }
            else{
                if (points[j].y - 1 == points[i].y){
                    rooms[2] = points[j].name;
                    }
                };
            };
        };
        for (std::string jonathan : rooms){
            outFile << jonathan << std::endl;
        };
    };
    outFile.close();
};