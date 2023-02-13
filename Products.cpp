#include"Products.hpp"
#include"Amount.hpp"
#include<iostream>

Products::Products(std::string name, int am) {
    name = name;
    amount = Amount(am);
};

Products::Products() {}

std::string Products::getName() {
    return name;
};

