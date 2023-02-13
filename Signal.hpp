#pragma once
#include"Amount.hpp"
#include<string>

class Signal {
    public:
        Signal(std::string, int);
        int getSignalAmount();
        std::string getName();
    private:
        std::string name;
        Amount amount;
};