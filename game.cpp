#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <functional>

struct room {
    std::string name;
    room* north;
    room* south;
    room* east;
    room* west;
};

const std::string filler = "You are in the ";
room* bedroom = new room;
room* loc = bedroom;
std::string nar;
bool yarnar;

bool valid(std::string yar){
    if (yar == "yar"){
        return true;
    };
    return false;
}

int main() {

char where;

std::string print = filler + loc->name;
std::cout << print << std::endl;
do {
    std::cout << "Which way bucko?" << std::endl;
    std::cin >> where;

    std::map<char, room* > directions = {
    {'s', loc->south},
    {'e', loc->east},
    {'w', loc->west},
    {'n', loc->north}
    };

    loc = directions[where];

    std::cout << "Wanna keep going?" << std::endl;
    std::cin >> nar;
    yarnar = valid(nar);

    std::string print = filler + loc->name;
    std::cout << print << std::endl;
    }
while (yarnar);
};