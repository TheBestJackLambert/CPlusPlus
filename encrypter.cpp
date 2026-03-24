#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cmath>

std::vector<int> chartoint(std::string txt){
    int count = 0;
    int sum = 0;
    std::vector<int> final;
    for(char j : txt){
        int i = (int) j;
        sum = sum + i * pow(128, count);
        count++;
        if(count == 4){
            final.push_back(sum); 
            sum = 0;
            count = 0;
        };
    };
    if (count != 0){
        final.push_back(sum);
    };
    return final;

};

std::string intotochar(std::vector<int> txt){
    std::string output = "";

    int place = 0;
    char character;
    int mod;
    for (int j: txt){ 
        std::string letter = "";
        for (int i = 3; i >= 0; i--){
            place = 0;
            mod = std::pow(128, i);
            place = j - j % mod;
            j = j - place;
            place = place / mod;
            character = (char) place;
            letter = letter + character;
        };
        std::reverse(letter.begin(),letter.end());
        output = output + letter;
    };
    return output;
};

std::string encrypt(std::string txt){
    while (txt.size() % 4 != 0){
        txt += ((char) 31);
    };
    auto input = chartoint(txt);
    std::reverse(input.begin(), input.end());
    for (auto& i: input){
        i = 266321790 - i + 67637280;
    };
    return intotochar(input);
};

std::string dencrypt(std::string txt){
    auto output = chartoint(txt);
    for (auto& i: output){
        i = 266321790 - i + 67637280;
    };
    std::reverse(output.begin(), output.end());
    auto readable = intotochar(output);
    while (readable.back() == (char) 31){
        readable.pop_back();
    };
    return readable;
};

int main(){
    std::string de = "";
    std::cout << "Encrypt or Deencrypt" << std::endl;

    while (de != "Encrypt" and de != "Dencrypt"){
        std::cin >> de;
    };

    std::string filename;
    std::string templae = "What file do you want to ";
    auto printing = templae + de;
    std::cout << printing << std::endl;

    std::cin >> filename;
    std::ifstream reading(filename);
    while(!reading){
        std::cout << "try again or ill eat you" << std::endl;
        std::cin >> filename;
        std::ifstream reading(filename);
    };

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