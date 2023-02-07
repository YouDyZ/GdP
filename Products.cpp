#include"Products.hpp"
#include"Amount.hpp"
#include<iostream>

Products::Products(std::string name, int am, int signal) {
    name = name;
    amount = Amount(am);
    signalAmount = Amount(signal);
};

Products::Products() {}

Products::~Products() {
    std::cout << name << "wurde gelöscht!" << std::endl;
}

std::string Products::getName() {
    return name;
};

