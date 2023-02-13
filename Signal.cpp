#include"Signal.hpp"
#include"Amount.hpp"
#include<string>

Signal::Signal(std::string name, int am) {
    name = name;
    amount = Amount(am);
};

int Signal::getSignalAmount() {
    return amount.get();
}

std::string Signal::getName() {
    return name;
}

