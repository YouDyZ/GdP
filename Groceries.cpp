#include"Groceries.hpp"
#include<string>
#include<iostream>

Gorceries::Gorceries(std::string name, int am, int signal, int expDate) {
    name = name;
    amount = Amount(am);
    signalAmount = Amount(signal);
    expireDate = expDate;
}

Gorceries::~Gorceries() {
    std::cout << name << "wurde entfernt!"<<std::endl;
}

int Gorceries::getExpireDate() {
    return expireDate;
};