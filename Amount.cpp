#include"Amount.hpp"
#include<iostream>

Amount::Amount(int am) {
    count = am;
};

int Amount::get() {
    return count;    
};

void Amount::set(int am) {
    count = am;
};

void Amount::increase(int am) {
    count += am;
};

void Amount::decrease(int am) {
    int fut = count - am;
    if(count > fut) {
        std::cout << "Mehr verbraucht als vorhanden!" << std::endl;
        return;
    } 
    if( count == fut) {
        std::cout<< "Gegenstand verbraucht!" << std::endl; 
        return;
    }
    count = fut;
};


//Standartkontruktor mit count = 0;
Amount::Amount () {
    count = 0;
}