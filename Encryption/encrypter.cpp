#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

int chartoint(std::string txt){

};

std::string intotochar(int txt){

};

std::string encrypt(std::string txt){

};

std::string dencrypt(std::string txt){

};

int main(){
    std::string de = "";
    std::cout << "Encrypt or Decrypt" << std::endl;

    while (de != "Encrypt" and de != "Dencrypt"){
        std::cin >> de;
    };

    std::string filename;
    std::string templae = "What file do you want to";
    auto printing = templae + de;
    std::cout << printing << std::endl;

    std::cin >> filename;
    std::fstream reading(filename);
    while(!reading){
        std::cout << "try again or ill eat you" << std::endl;
        std::cin >> filename;
        std::fstream reading(filename);
    }

    std::string thingtobeencrypted;
    std::string input = "";
    while(std::getline(reading, thingtobeencrypted)){
        input = input + thingtobeencrypted;
    };
    

    std::string outputloc;

    std::cout << "Where do you want the output to go?";
    std::cin >> outputloc;

    std::string output;
    if (de == "Encrypt"){
        output = encrypt(input);
    }
    else{
        output = dencrypt(input);
    };

    reading.close();
    std::cout << output;
    std::ofstream outFile(outputloc);
    outFile << output << std::endl;
};