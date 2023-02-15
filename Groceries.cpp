#include"Groceries.hpp"
#include<string>
#include<iostream>
#include<sstream>

Gorceries::Gorceries(std::string iname, int am, std::tm expDate) {
    name = iname;
    amount = Amount(am);
    expireDate = expDate;
}


std::string Gorceries::getExpireDateString() {
    int day = expireDate.tm_mday;
    int month = expireDate.tm_mon+1;
    int year = expireDate.tm_year+1900;
    std::stringstream output;
    output << day << "." << month << "." << year;
    return output.str();
}

std::chrono::system_clock::time_point Gorceries::getExpireDate() {
    
    return std::chrono::system_clock::from_time_t(std::mktime(&expireDate));
};