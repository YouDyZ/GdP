#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<chrono> //TODO ggf chrono deleten!
#include"Groceries.hpp"
#include"Products.hpp"


void log(std::string message, bool nl = false) {
    std::cout << message;
    if(nl) {
        std::cout << std::endl;
    }
}

int menue() {
    int input;
    //[ ] Kontrolle input == type char || String
    do {
        log("--------------------------------", true);
        log("1: Neues Item einfuegen", true);
        log("2: Items verbrauchen", true);
        log("3: Items in Datei Speichern", true);
        log("4: Items aus Datei einlesen");
        log("0: Exit", true);
        log("--------------------------------", true);
        std::cin >> input;
    } while (input < 0 || input > 4);
    return input;
};

void addNewItem();
void useItem();
void saveItems();
void insertFromFile();

std::vector<Products> prodcuts;
std::vector<Gorceries> gorceries;

int main() {
    bool running = true;
    do
    {
        int input = menue();
        std::cout << input << std::endl;
        switch (input) {
            case 1:
                addNewItem();
                break;
            case 2:
                useItem();
                break;
            case 3:
                saveItems();
                break;
            case 4:
                insertFromFile();
                break;
            case 0:
                running = false;
                break;
            default:
                break;
    }
    } while (running);
    
    return 0;
};

void addNewItem() {
    bool isGrocery = false;
    char input;
    bool succses = false;
    do {
        do {
            std::cout << "Ist das Item verderblich? [Y/N]"<<std::endl;
            std::cin >> input;
        } while(input == 'Y' || input == 'y' || input == 'n' || input == 'N');
        if(input == 'Y' || input == 'y') {
            isGrocery = true;     
        }
        bool confirm = false;
        std::string prodName;
        int prodAmount;
        int signalAmount;

        //Produkt name abfragen + bestätigen
        do {
            char in;
            std::cout << "Wie ist der Name des Produkt?" << std::endl;
            std::cin >> prodName;
            std::cout << "Der name des Produkt ist also: " << prodName << "[Y/N]" <<std::endl; 
            std::cin >> in;
            if(in == 'y' || in == 'Y') {
                confirm = true;
            }
        } while (!confirm);
        //[ ] Product in Database

        confirm = false;
        do {
            char in;
            std::cout << "Wie häufig haben Sie das Produkt?" << std::endl;
            std::cin >> prodAmount;
            std::cout << "Produkt " << prodAmount << "x vorhanden? [Y/N]" <<std::endl; 
            std::cin >> in;
            if(in == 'y' || in == 'Y') {
                confirm = true;
            }
        } while (!confirm);
        confirm = false;
        do {
            char in;
            std::cout << "Ab wie viel Restmänge wollen Sie erinnert werden?" << std::endl;
            std::cin >> signalAmount;
            std::cout << "Erinnerung bei " << signalAmount << "x vorhanden? [Y/N]" <<std::endl; 
            std::cin >> in;
            if(in == 'y' || in == 'Y') {
                confirm = true;
            }
        } while (!confirm);
        confirm = false;
        if(isGrocery) {
            int expDateDay;
            int expDateMonth;
            int expDateYear;
            do {
                char in;
                bool formatRight = false;
                do {
                    std::cout << "Wann läuft das Produkt ab? "<< std::endl << "Tag: ";
                    std::cin >> expDateDay;
                    std::cout << "Moant: ";
                    std::cin >> expDateMonth;
                    std::cout << "Jahr: ";
                    std::cin >> expDateYear;
                    //std::chrono::year_month_weekday; //[ ] Day, Month Year to int
                } while(!formatRight);
                //std::cout << "Produkt läuft " << expDate << "ab? [Y/N]" <<std::endl; 
                std::cin >> in;
                if(in == 'y' || in == 'Y') {
                    confirm = true;
                }
            } while (!confirm);
            //Gorceries addedItem = Gorceries(prodName, prodAmount, signalAmount, expDateInt);
            //gorceries.push_back(Gorceries(prodName, prodAmount, signalAmount, expYear);
            //[ ] Create Grocery and add it to groceries<>
        } else {
            prodcuts.push_back(Products(prodName, prodAmount, signalAmount));
            //[ ] Create Product and add it to products<>
        }
    } while (!succses);

};

void useItem() {
    bool confirm = false;
    std::string prodName;
    do {
        char in;
        std::cout << "Wie ist der Name des Produkts?" << std::endl; 
        std::cin >> prodName;
        std::cout << "Ist " << prodName << "richtig? [Y/N]" << std::endl;
        std::cin >> in;

        if(in == 'y' || in == 'Y') {
            confirm = true;
        } 
    } while(!confirm);
    confirm = false;
    std::vector<int> resProd;
    std::vector<int> resGroc;

    for (size_t i = 0; i < prodcuts.size(); i++) {
        if(prodcuts.at(i).getName() == prodName) {
            resProd.push_back(i);
        }
    }
     for (size_t i = 0; i < gorceries.size(); i++) {
        if(gorceries.at(i).getName() == prodName) {
            resGroc.push_back(i);
        }
    }
    
    if(resGroc.size()+resProd.size() == 0 ) {
        std::cout << "Keine Produkte Gefunden" << std::endl;
    } else {
        int listNum = 1;
        if(resProd.size() != 0 ) {
            for (int k = 0; k < resProd.size(); k++) {
                Products prod = prodcuts.at(resProd.at(k));
                std::cout   << listNum
                            << ": " 
                            << prod.getName()
                            << ", "
                            << prod.amount.get()
                            << "x "
                            <<std::endl;
                listNum++;
            }
        }
        if(resGroc.size() != 0) {
            for (int k = 0; k < resGroc.size(); k++) {
                Gorceries prod = gorceries.at(resGroc.at(k));
                std::cout   << listNum
                            << ": " 
                            << prod.getName()
                            << ", "
                            << prod.amount.get()
                            << "x "
                            <<std::endl;
                listNum++;
            }
        }
        int selectItem;
        bool next = false;
        do {
            std::cout << "Bitte Nummer eingeben!" << std::endl;
            std::cin >> selectItem;
            if(!(selectItem != 0) || !(selectItem > (resProd.size() + resGroc.size()))) {
                next = true;
            } else {
                std::cout << "Bitte eingabe überprüfen!" << std::endl;
            }
        } while (!next);
        next = false;
        if(selectItem == 0) {
            return;
        } else {
            int decreaseAm;
            bool isGroc;
            int itemIndex;
            if(selectItem > resProd.size()) {
                isGroc = true;
                itemIndex = resGroc.at(selectItem -1);
            } else {
                itemIndex = resProd.at(selectItem -1);
            }
            do {
                std::cout << "Wie viel wurde verbaucht?" << std::endl;
                std::cin >> decreaseAm;
                if(isGroc) {
                    int max = gorceries.at(itemIndex).amount.get();
                    if(decreaseAm > max) {
                        std::cout << "Sie koennen nicht mehr verauchen, als was Sie haben!" << std::endl;
                    } else if(decreaseAm < 0) {
                        char confirmAdd;
                        std::cout << "Sie können keinen Negativen verbauchen haben!" << std::endl;
                        do {
                           std::cout << "Wollen Sie statdessen den betrag hinzufügen? [Y/N]" << std::endl;
                        } while (confirmAdd == 'Y' || confirmAdd == 'y' || confirmAdd == 'n' || confirmAdd == 'N');
                        if(confirmAdd == 'Y' || confirmAdd == 'N') {
                            gorceries.at(itemIndex).amount.increase(-decreaseAm);
                        }
                    } else if(decreaseAm == max){
                        std::cout << "Gegenstand komplett veraucht." << std::endl;
                        gorceries.erase(gorceries.begin() + itemIndex);
                        next == true;
                    } else if(decreaseAm == 0) {
                        std::cout << "Eintag bleibt Unveraendert" << std::endl;
                        next = true;
                    } else {
                        gorceries.at(itemIndex).amount.decrease(decreaseAm);
                        next == true;
                    }
                } else {
                    int max = prodcuts.at(itemIndex).amount.get();
                    if(decreaseAm > max) {
                        std::cout << "Sie koennen nicht mehr verauchen, als was Sie haben!" << std::endl;
                    } else if(decreaseAm < 0) {
                        char confirmAdd;
                        std::cout << "Sie können keinen Negativen verbauchen haben!" << std::endl;
                        do {
                           std::cout << "Wollen Sie statdessen den betrag hinzufügen? [Y/N]" << std::endl;
                        } while (confirmAdd == 'Y' || confirmAdd == 'y' || confirmAdd == 'n' || confirmAdd == 'N');
                        if(confirmAdd == 'Y' || confirmAdd == 'N') {
                            prodcuts.at(itemIndex).amount.increase(-decreaseAm);
                        }
                    } else if(decreaseAm == max){
                        std::cout << "Gegenstand komplett veraucht." << std::endl;
                        prodcuts.erase(prodcuts.begin() + itemIndex);
                        next == true;
                    } else if(decreaseAm == 0) {
                        std::cout << "Eintag bleibt Unveraendert" << std::endl;
                        next = true;
                    } else {
                        prodcuts.at(itemIndex).amount.decrease(decreaseAm);
                        next == true;
                    }
                }        
            } while (!next);
        }
    }
};

void saveItems() {
    std::string fileName;
    std::cout << "Datei wird als Textdatei gespeichert!" << std::endl;
    bool nameValid = false;
    do {
        std::cout << "Geben Sie einen Dateinamen an!" << std::endl;
        std::cin >> fileName;
        //[ ] Namenvalidierung auf . und existierende Files
    } while (!nameValid);
    
    std::ofstream fout;
    fout.open(fileName + ".txt");
    if(prodcuts.size() != 0) {
        fout << "--Nicht verderbliche Produkte--" << std::endl;
        fout << std::endl;
        for (size_t i = 0; i < prodcuts.size(); i++) {
            fout << "Produkt: " << prodcuts.at(i).getName() << std::endl;
            fout << "Anzahl: " << prodcuts.at(i).amount.get() << std::endl;
            fout << std::endl;
        };
        fout << std::endl;
    }
    
    if(gorceries.size() != 0) {
        fout << "--Nicht verderbliche Produkte--" << std::endl;
        fout << std::endl;
        for (size_t i = 0; i < gorceries.size(); i++) {
            fout << "Produkt: " << gorceries.at(i).getName() << std::endl;
            fout << "Anzahl: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Haltbar bis: " << gorceries.at(i).getExpireDate() << std::endl;
            fout << std::endl;
        }
    }
    fout.close();
    std::cout << "Datei: " << fileName << ".txt wurde geschrieben" << std::endl; 
};

void insertFromFile() {
    //[ ] von datei Einlesen
}