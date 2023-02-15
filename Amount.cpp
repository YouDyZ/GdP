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
    count = count - am;
};


//Standartkontruktor mit count = 0;
Amount::Amount () {
    count = 0;
}