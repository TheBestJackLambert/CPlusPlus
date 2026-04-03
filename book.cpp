#include <ncurses.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <set>
#include <iostream>
#include <optional>

class contact{
    public:
    std::map<std::string, std::string> information;

    std::optional<std::string> get(std::string request){
        if (information.contains(request)){
            return information.at(request);
        };
        return std::nullopt;
    };

    void edit(std::string type, std::string info){

        if (information.contains(type)){
            information[type] = info;
        }
        else{
            information.insert({type, info});
        };
    };

    void remove(std::string type){
        if (information.contains(type)){
            information.erase(type);
        };
    };
};


std::map <std::string, contact> contacts;
std::vector <std::string> tempcontact;
std::string content;
std::string identifier;
std::string line;
char typed[255];
std::string input;
std::string contactinput;
std::string newpart;
bool swap;

void home(){
    clear();
    attron(A_BOLD);
    addstr("Contact Book");
    addstr("\n");
    addstr("Contacts:");
    addstr("\n");
    attroff(A_BOLD);
    for (auto [label, contacter] : contacts){
        addstr((label + "\n").c_str());
    };
    refresh();
};

void edit(std::string person, std::string component, std::string replacement){
    contacts[person].edit(component, replacement);
    if (component == "name"){
        contact temp = contacts[person];
        contacts.erase(person);
        contacts.insert({replacement, temp});
    };
};

void showcontact(std::string input){
    attron(A_BOLD);
    addstr("Name: \n");
    attroff(A_BOLD);
    addstr((input + "\n").c_str());
    for(auto [label, info] : contacts[input].information){
        if (label == "name"){
                continue;
            };
        attron(A_BOLD);
        addstr((label + "\n").c_str());
        attroff(A_BOLD);
        addstr((info + "\n").c_str());
    };
};

void opencontact(std::string input){
    clear();
    showcontact(input);
    refresh();
    getstr(typed);
    contactinput = std::string(typed);
    if (contactinput == "edit"){
        clear();
        showcontact(input);
        addstr("What do you want to edit?\n");
        refresh();
        getstr(typed);
        contactinput = std::string(typed);
        refresh();
        clear();
        showcontact(input);
        addstr((contactinput + ": ").c_str());
        refresh();
        getstr(typed);
        newpart = std::string(typed);
        edit(input, contactinput, newpart);
        if (contactinput != "name"){
            opencontact(input);
        }
        else{
            opencontact(newpart);
        };
        refresh();
    }
    else{
        if (contactinput == "remove"){
            clear();
            showcontact(input);
            addstr("What do you want to remove?\n");
            refresh();
            getstr(typed);
            contactinput = std::string(typed);
            if (contactinput != "name"){
                contacts[input].remove(contactinput);
            }
            opencontact(input);
        }
        else{
            home();
        };
    };
};

int main(){
    std::ifstream read("contacts.txt");

    contact readin;
    readin = contact{};
    std::string name;
    while (getline(read, line)){
        if (line.find(':') == std::string::npos){
            continue;
        };
        std::string temp1 = "";
        std::string temp2 = "";
        swap = true;
        for(char i: line){
            if (swap){
                if (i == ':'){
                    swap = false;
                }
                else{
                    temp1 = temp1 + (i);
                };
            }
            else{
                if(temp2 == ""){
                    if(i != ' '){
                        temp2 = temp2 + (i);
                    };
                }
                else{
                    temp2 = temp2 + (i);
                };
            };
                
        };
        if (temp1 == "name"){
            contacts.insert({temp2, readin});
            name = temp2;
        };
        contacts[name].edit(temp1, temp2);

    };
    initscr();
    home();
    while(true){
        refresh();

        getstr(typed);
        input = std::string(typed);

        if (contacts.contains(input)){
            opencontact(input);
        }
        else{
            if (input == "create"){
                clear();
                home();
                addstr("What is their name?\n");
                refresh();
                getstr(typed);
                input = std::string(typed);
                contact created;
                contacts.insert({input, created});
                edit(input, "name", input);
                opencontact(input);
            }
            else {
                if (input == "delete"){
                    clear();
                    home();
                    refresh();
                    addstr("Who do you want to delete?\n");
                    getstr(typed);
                    input = std::string(typed);
                    if (contacts.contains(input)){
                        contacts.erase(input);
                    };
                    home();
                    refresh();
                }
                else{
                    break;
                }
            };
        };
    };
    endwin();
    read.close();
    std::ofstream file("contacts.txt");
    file << "|" << std::endl;
    for (auto [label, contacter] : contacts){
        file << ("name: " + label) << std::endl;
        for(auto [tag, info] : contacter.information){
            if (tag == "name"){
                continue;
            };
            file << (tag + ": " + info) << std::endl;
        };
        file << "|" << std::endl;
    };
};