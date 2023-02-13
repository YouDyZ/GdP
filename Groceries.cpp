#include"Groceries.hpp"
#include<string>
#include<iostream>

Gorceries::Gorceries(std::string name, int am, std::tm expDate) {
    name = name;
    amount = Amount(am);
    expireDate = expDate;
}

Gorceries::~Gorceries() {
    std::cout << name << "wurde entfernt!"<<std::endl;
}

std::string Gorceries::getExpireDateString() {
    int day = expireDate.tm_mday;
    int month = expireDate.tm_mon + 1;
    int year = expireDate.tm_year + 1990;
    std::string output = day + "." +  
    output += month + ".";
    output += + year;
    return output;
}

std::chrono::system_clock::time_point Gorceries::getExpireDate() {
    return std::chrono::system_clock::from_time_t(std::mktime(&expireDate));
};