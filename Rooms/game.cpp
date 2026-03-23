#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <fstream>

std::fstream rooms("rooms.txt");
int samantha = 0;

struct room {
    std::string name;
    room* north;
    room* south;
    room* east;
    room* west;
};

const std::string filler = "You are in the ";
std::string nar;
bool yarnar;

bool valid(std::string yar){
    if (yar == "yar"){
        return true;
    };
    return false;
};

int main() {
    std::map<std::string, room* > strings;

    std::string line;
    while (std::getline(rooms, line)){
        if (line == "|"){
            samantha = 0;
        }
        else{
            if (samantha == 0){
                std::string temp = line;
                room* roberto = new room {line, nullptr, nullptr, nullptr, nullptr};
                strings.insert({line, roberto});
            };
            samantha++;
        };
    };

    rooms.close();
    std::fstream rooms("rooms.txt");

    std::string otherline;
    std::string tempname;
    while (std::getline(rooms, otherline)){
        if (otherline == "|"){
            samantha = 0;
        }
        else{
            if (otherline != "nullptr"){
                if (samantha == 0){
                    tempname = otherline;
                };
                if (samantha == 1){
                    strings[tempname]->north = strings[otherline];
                };
                if (samantha == 2){
                    strings[tempname]->east = strings[otherline];
                };
                if (samantha == 3){
                    strings[tempname]->south = strings[otherline];
                };
                if (samantha == 4){
                    strings[tempname]->west = strings[otherline];
                };
            }
            samantha++;
        };
    };
char where;

room* loc;
std::string starting = "";
while (strings.count(starting) ==0){
    std::cout << "Where do you want to start?" << std::endl;
    std::cin >> starting;
};

loc = strings[starting];

std::string print = filler + loc->name;
std::cout << print << std::endl;
while (true) {
    std::cout << "Which way bucko?" << std::endl;
    std::cin >> where;

    if(where == 'x'){
        return 0;
    };

    std::map<char, room* > directions = {
    {'s', loc->south},
    {'e', loc->east},
    {'w', loc->west},
    {'n', loc->north}
    };

    if (directions[where] != nullptr){
    loc = directions[where];
    std::string print = filler + loc->name;
    std::cout << print << std::endl;
    }
    else{
        std::cout << "Nope" << std::endl;
    };

    };
};