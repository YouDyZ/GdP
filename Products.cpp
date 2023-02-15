#include"Products.hpp"
#include"Amount.hpp"
#include<iostream>

Products::Products(std::string iname, int am) {
    name = iname;
    amount = Amount(am);
};

Products::Products() {};

std::string Products::getName() {
    return name;
};

