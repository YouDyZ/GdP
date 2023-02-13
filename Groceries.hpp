#pragma once
#include"Products.hpp"
#include<chrono>
#include<ctime>
#include<string>
class Gorceries: public Products {
    public: 
        Gorceries(std::string, int, std::tm expDate);
        ~Gorceries();  
        std::chrono::system_clock::time_point getExpireDate();
        std::string getExpireDateString();
    private:
        std::tm expireDate;
};
