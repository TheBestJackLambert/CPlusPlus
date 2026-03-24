#include <ncurses.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>

class contact{

    private:

    std::string name;
    std::string number;
    std::string email;
    std::string notes = "";
    std::vector<bool> filled = {0,0,0,0};

    public:

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
        name = username;
        filled[0] = true;
    };

    void emailmaker(std::string mail){
        email = mail;
        filled[1] = true;
    };

    void numbermaker(std::string numbo){
        number = numbo;
        filled[2] = true;
    };

    void noter(std::string note){
        notes = notes + note + '\n';
        filled[3] = true;
    };

    void removenote(){
        notes = "";
        filled[3] = false;
    };
};


std::vector <contact> contacts;
std::vector <std::string> tempcontact;
std::string content;


int main(){
    std::fstream contactfile("contacts.txt", std::ios::in | std::ios::out);
    while (std::getline(contactfile, content)){

    };

    initscr();
    clear();
    attron(A_BOLD);
    addstr("Contact Book");
    addstr("\n");
    addstr("Contacts:");
    attroff(A_BOLD);
    for (auto i:contacts){
        addstr((i.get("name") + "\n").c_str());
    };
    refresh();
    getch();
    endwin();
};