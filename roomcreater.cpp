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
    int x;
    int y;
    std::string name;
};

std::vector<grid> points = {};

int main() {
    std::string name;
    int x;
    int y;

    while (john){
    std:: cin >> x;
    std:: cin >> y;
    std:: cin >> name;
    grid robert = grid{x, y, name};
    points.push_back(robert);
    count++;
    };
    for(int i = 0; i == count; i++){
        for(int j = 0; j == count; j++){
            if (points[j].x + 1 == points[i].x){

            }
            else{
                if (points[j].x + 1 == points[i].x){

                }
                else{
                    if (points[j].x + 1 == points[i].x){

                    }
                    else{
                        if (points[j].x + 1 == points[i].x){

                        };
                    };
                };
            };
        };
    };
};