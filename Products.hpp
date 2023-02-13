#pragma once
#include"Amount.hpp"
#include<string>

class Products {
    protected:
        std::string name;
        
    public:
        Products(std::string, int);
        Products();
        std::string getName();
        Amount amount;
};

