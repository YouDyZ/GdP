#pragma once
#include"Products.hpp"

class Gorceries: Products {
    public: 
        Gorceries(std::string, int, int, int);
        ~Gorceries();  
        int getExpireDate();
    private:
        int expireDate;
};
