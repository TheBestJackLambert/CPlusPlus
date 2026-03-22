#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <functional>


class number{
    private:
    float num;

    public:
    void def(){
        std::cout << "Number?" << std::endl;
        if (std::cin >> num){}
        else {
            std::cout << "I hate you >:(" << std::endl;
            std::cin.clear();
            num = 0;
        };
    };

    void add(float num2){
        num = num + num2;
    };

    void div(float num2)
    {
        if (num2 == 0)
        {
            std::cout << "I hate you >:(" << std::endl;
            return;
        };
        num = num / num2;
    };

    void sub(float num2){
        num = num - num2;
    };

    void mult(float num2){
        num = num * num2;
    };
    float value() const {
        return num;
    };
    };
const std::string oplist[4] = {"add", "sub", "mult", "div"};
bool valid(std::string str) {
    const auto& ops = oplist;
    for(auto i : ops){
    if (i == str){return true;}};
    return false;
};

int main() {
    number num;
    num.def();
    float num2;

    std::map<std::string, std::function<void(float)>> operations;
    operations["add"]  = [&num](float n) { num.add(n); };
    operations["sub"]  = [&num](float n) { num.sub(n); };
    operations["mult"] = [&num](float n) { num.mult(n); };
    operations["div"]  = [&num](float n) { num.div(n); };
    std::string op;

   do
    {
        std::cout << "What operation?" << std::endl;
        std::cin >> op;
        if (valid(op)){
            std::cout << "What number?" << std::endl;
            std::cin >> num2;
            operations[op](num2);
            std::cout << num.value() << std::endl;
        }

    } while (valid(op));

    std::cout << num.value() << std::endl;

};