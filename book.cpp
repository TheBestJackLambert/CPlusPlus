#include <ncurses.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <set>
#include <iostream>

class contact{

    private:

    std::string name;
    std::string number;
    std::string email;
    std::string notes = "";

    public:
    std::vector<bool> filled = {0,0,0,0};

    void initialize(std::string na, std::string nu, std::string em, std::string no){
        edit("name", na);
        edit("email", em);
        edit("number", nu);
        edit("notes", no);
    };

    std::string get(std::string request){
        std::map<std::string, std::string> items = {
        {"name", name},
        {"email", email},
        {"number", number},
        {"notes", notes}
        };

        return items.at(request);
    }

    std::string obtain(int request){
        std::map<int, std::string> items = {
        {0, name},
        {1, email},
        {2, number},
        {3, notes}
        };
        return items.at(request);
    }


    void edit(std::string type, std::string info){

        std::map<std::string, std::function<void(std::string)>> typecreate = {
        {"name", [this](std::string info){namemaker(info);} },
        {"email", [this](std::string info){emailmaker(info);} },
        {"number", [this](std::string info){numbermaker(info);} },
        {"notes", [this](std::string info){noter(info);} }
        };

        typecreate.at(type)(info);
    };

    void namemaker(std::string username){
        if(username != ""){
            name = username;
            filled[0] = true;
        };
    };

    void emailmaker(std::string mail){
        if(mail != ""){
            email = mail;
            filled[1] = true;
        };
    };

    void numbermaker(std::string numbo){
        if(numbo != ""){
            number = numbo;
            filled[2] = true;
        };
    };

    void noter(std::string note){
        if(note != ""){
            notes = notes + note + '\n';
            filled[3] = true;
        };
    };

    void removenote(){
        notes = "";
        filled[3] = false;
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
std::set<std::string> options = {"name", "email", "number", "notes"};
std::string newpart;
bool swap;

std::map <int, std::string> labels = {
    {0, "Name:"},
    {1, "Email:"},
    {2, "Number:"},
    {3, "Notes:"}
};
std::map <int, std::string> labe = {
    {0, "name:"},
    {1, "email:"},
    {2, "number:"},
    {3, "notes:"}
};
void home(){
    clear();
    attron(A_BOLD);
    addstr("Contact Book");
    addstr("\n");
    addstr("Contacts:");
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
    for(int i = 0; i < contacts[input].filled.size(); i++){
        if (contacts[input].filled[i]){
            attron(A_BOLD);
            addstr((labels[i] + "\n").c_str());
            attroff(A_BOLD);
            addstr((contacts[input].obtain(i) + "\n").c_str());
        };
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
        if (!options.contains(contactinput)){
            home();
            return;
        };
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
        home();
    };
};

int main(){
    std::ifstream read("contacts.txt");

    contact readin;
    while (getline(read, line)){
        if (line[0] == '|'){
            contacts.insert({readin.get("name"), readin});
            readin = contact{};
            continue;
        }
        else{
            if (line.find(':') == std::string::npos){
                continue;
            };
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
        readin.edit(temp1, temp2);
    };
    if (readin.filled[0]){
        contacts.insert({readin.get("name"), readin});
        readin = contact{};
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
    for (auto [label, contacter] : contacts){
        for(int i = 0; i < contacter.filled.size(); i++){
            if (contacts[label].filled[i]){
                file << (labe[i] + contacter.obtain(i)) << std::endl;
            };
        };
        file << "|" << std::endl;
    };
};