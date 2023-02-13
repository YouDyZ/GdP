#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<chrono>
#include<ctime>
#include"Groceries.hpp"
#include"Products.hpp"
#include"Signal.hpp"

void addNewItem();
void useItem();
void saveItems();
void insertFromFile();
void init();
void log(std::string, bool);
int menue();
void readFromFile(std::string, bool);
void checkToEat();
void checkAmountLow();
void readSignalAmount();
std::chrono::system_clock::time_point today;
std::vector<Products> prodcuts;
std::vector<Gorceries> gorceries;
std::vector<Signal> signalAmounts;

int main() {
    bool running = true;
    std::cout << "Starte das Programm..." << std::endl;
    init();
    do {
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

void log(std::string message, bool nl = false) {
    std::cout << message;
    if(nl) {
        std::cout << std::endl;
    }
}

void readFromFile(std::string fileName, bool append = false) {
    if(!append) {
        prodcuts.clear();
        gorceries.clear();
    }
    std::ifstream file(fileName);
    std::string fileLine;
    std::vector<Products> tempProd;
    std::vector<Gorceries> tempGorc;

    while (std::getline(file, fileLine)) {
        std::stringstream sstream(fileLine);
        std::string valueType, value;
        sstream >> valueType;
        sstream >> value;

        std::string itemName;
        std::string expDate;
        int amount;

        //TODO DEBUG
        std::cout << "valueType: " << value;

        if((value != "")) {
            if(valueType == "Name:") {
                itemName = value;
            } else if(valueType == "Ablaufdatum:") {
                expDate = value;
            } else if(valueType == "Menge:") {
                amount = stoi(value);
            } else {
                std::cout << "Fehler in der Datei. Bitte Datei: '" << fileName << "' überprüfen!" << std::endl;
                return;
            }
        }

            //gegenstand hat meldebestand
            bool hasSignalAmount = false;
            for (int i = 0; i < signalAmounts.size(); i++) {
                if(itemName == signalAmounts.at(i).getName()) {
                    hasSignalAmount = true;
                }
            }
            int setSignalAmount;
            if(!hasSignalAmount) {
                do {
                    std::cout << "Bitte geben Sie ein Meldebestand für " << itemName << " ein!" << std::endl;
                    std::cin >> setSignalAmount;
                } while(setSignalAmount < 0);
                signalAmounts.push_back(Signal(itemName, setSignalAmount));
            }

            if(expDate.size() != 0 || expDate != "keines") {
                //ist ein Gorcerie
                int expDateDay, expDateMonth,expDateYear;
                char dummy1, dummy2;
                std::stringstream sstream(expDate);

                sstream >> expDateDay >> dummy1 >> expDateMonth >> dummy2 >> expDateYear;
                gorceries.push_back(Gorceries(itemName, amount, {0,0,0, expDateDay, expDateMonth-1, expDateYear-1900}));
            } else {
                prodcuts.push_back(Products(itemName, amount));
            }
            //reset for next loop
                    std::string itemName = "";
                    std::string expDate = "";
                    amount = 0;
    }
}

/**
 * Zuerst Signalmängen einlesen, dann alle vorhanden einlesen.
 * Listen aktuallisieren
 */
void init() {
    std::cout << "Daten werden gelesen aus data.txt" << std::endl;
    readSignalAmount();
    readFromFile("data.txt");

    std::cout << "Liste mit bald zu essenden Produkten wird aktuallisiert" << std::endl;
    
    today = std::chrono::system_clock::now();
    checkToEat();

    std::cout << "Datei wird aktuallisiert, mit pordukten unter Signalanzahl" << std::endl;
    checkAmountLow();
};


//[ ] Daten auf Dopplung überpüfen
void readSignalAmount() {
    std::ifstream file("singal.txt");
    std::string fileLine;
    bool reWriteFile = false;
    std::vector<int> removeAfter;
    while (std::getline(file, fileLine)) {
        std::stringstream sstream(fileLine);
        std::string valueName;
        std::string value;
        sstream >> valueName;
        sstream >> value;
        if(signalAmounts.size() == 0) {
            signalAmounts.push_back(Signal(valueName, std::stoi(value)));
        } else {
            //Dopplung Überpüfen
            for (int i = 0; i < signalAmounts.size(); i++) {
                if (signalAmounts.at(i).getName() == valueName && signalAmounts.at(i).getSignalAmount() == std::stoi(value)) {
                    std::cout << "Dopplung bei den Signalwerten vorhanden. Bitte Dopplung enfernen um diese Meldung zu deaktivieren." << std::endl <<  "Item name: " << valueName << std::endl;
                    if(!reWriteFile) {
                        char reWrite;
                        do {
                            std::cout << "Wollen Sie die Datei im Anschluss neu erzeugen? [Y/N]" << std::endl;
                            std::cin >> reWrite;
                        } while (reWrite == 'Y' || reWrite == 'y' || reWrite == 'n' || reWrite == 'N');
                        if(reWrite == 'Y' || reWrite == 'y') {
                            reWriteFile = true;
                        }
                    }
                } else if(signalAmounts.at(i).getName() == valueName) {
                    int settedSignal;
                    do {
                        std::cout << "Dopplung bei den Signalwerten vorhanden. Unterschiedliche Signalwerte. Bitte auswählen" << std::endl;
                        std::cout << valueName << std::endl;
                        std::cout << "1: " << value << std::endl;
                        std::cout << "2: " << signalAmounts.at(i).getSignalAmount() << std::endl;
                        std::cin >> settedSignal;
                    } while (!(settedSignal == 1 || settedSignal == 2));
                    if(settedSignal == 1) {
                        //TODO Remove after finishing all loops
                        removeAfter.push_back(i);
                        signalAmounts.push_back(Signal(valueName, std::stoi(value)));
                    } 
                    if(!reWriteFile) {
                        char reWrite;
                        do {
                            std::cout << "Wollen Sie die Datei im Anschluss neu erzeugen? [Y/N]" << std::endl;
                            std::cin >> reWrite;
                        } while (reWrite == 'Y' || reWrite == 'y' || reWrite == 'n' || reWrite == 'N');
                        if(reWrite == 'Y' || reWrite == 'y') {
                            reWriteFile = true;
                        }
                    }
                } else {
                    signalAmounts.push_back(Signal(valueName, std::stoi(value)));
                }
            }
        }
    }
    if(reWriteFile) {
        std::cout << "signal.txt wird neu erzeugt..." << std::endl;
        remove("singal.txt");
        std::ofstream fout;
        fout.open("singal.txt");
        for (int i = 0; i < signalAmounts.size(); i++) {
            fout << signalAmounts.at(i).getName() << " " << signalAmounts.at(i).getSignalAmount() << std::endl;
        }
        fout.close();
        std::cout << "signal.txt erfolgreich erneut erzeugt" << std::endl;
    }
};

void checkToEat() {
    std::vector<Gorceries> nearExpDate;
    std::chrono::system_clock::time_point nowInSevenDays = today + std::chrono::seconds(7*24*60*60);
    for (int i = 0; i < gorceries.size(); i++) {
        Gorceries prod = gorceries.at(i);
        if(prod.getExpireDate() > nowInSevenDays) {
            nearExpDate.push_back(prod);
        }
    }
    std::cout << "Erstelle Datei mit bald verderbenden Produkten..." << std::endl;
    
    //[x] Datei aus verderbliche produkten erstellen.

    std::ofstream fout;
    fout.open("baldEssen.txt");
    fout << "--Bald zu essende Produkte--" << std::endl;
    for (int i = 0; i < nearExpDate.size(); i++) {
        fout << "Produkt: " << nearExpDate.at(i).getName() << std::endl;
        fout << "Anzahl: " << nearExpDate.at(i).amount.get() << std::endl;
        fout << "Ablaufdatum: " << nearExpDate.at(i).getExpireDateString() << std::endl;
    }
    fout.close();
    std::cout << "Datei mit Bald zu essenden Produkten als 'baldEssen.txt' gespeichert!" << std::endl;
};


//[ ] Auf daten aus liste Überarbeiten
void checkAmountLow() {
    std::ofstream fout;
    fout.open("nachkaufen.txt");
    fout << "--Unerderbliche Produkte--" << std::endl;
    for (int i = 0; i < prodcuts.size(); i++) {
        if(prodcuts.at(i).amount.get()) {
            fout << "Produkt: " << prodcuts.at(i).getName() << std::endl;
            fout << "Anzahl: " << prodcuts.at(i).amount.get() << std::endl;
            fout << "" << std::endl;
        }
    }
    fout << "" << std::endl;
    fout << "--Verderbliche Produkte--" << std::endl;
    for (int i = 0; i < gorceries.size(); i++) {
        if(gorceries.at(i).amount.get() < 200) {
            fout << "Produkt: " << gorceries.at(i).getName() << std::endl;
            fout << "Anzahl: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Ablaufdatum: " << gorceries.at(i).getExpireDateString() << std::endl;
            fout << "" << std::endl;
        }
    }
    fout.close();
    std::cout << "Produkte mit niedrigem Bestand in nachkaufen.txt geschrieben" << std::endl;
}

int menue() {
    int input;
    //[ ] Datenüberprüfung int
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
        if(isGrocery) {
            int expDateDay;
            int expDateMonth;
            int expDateYear;
            std::tm expDate;
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
                    expDate = {0,0,0, expDateDay, expDateMonth-1, expDateYear-1900};
                } while(!formatRight);
                std::cout << "Produkt läuft "   << expDate.tm_mday << "." 
                                                << expDate.tm_mon + 1  << "." 
                                                << expDate.tm_year + 1990 
                                                << "ab? [Y/N]" <<std::endl;
                std::cin >> in;
                if(in == 'y' || in == 'Y') {
                    confirm = true;
                }
            } while (!confirm);
            gorceries.push_back(Gorceries(prodName, prodAmount, expDate) );
        } else {
            prodcuts.push_back(Products(prodName, prodAmount));
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
                           std::cout << "Wollen Sie statdessen den Betrag hinzufügen? [Y/N]" << std::endl;
                           std::cin >> confirmAdd;
                        } while (confirmAdd == 'Y' || confirmAdd == 'y' || confirmAdd == 'n' || confirmAdd == 'N');
                        if(confirmAdd == 'Y' || confirmAdd == 'N') {
                            gorceries.at(itemIndex).amount.increase(-decreaseAm);
                        }
                    } else if(decreaseAm == max){
                        std::cout << "Gegenstand komplett veraucht." << std::endl;
                        gorceries.erase(gorceries.begin() + itemIndex);
                        next = true;
                    } else if(decreaseAm == 0) {
                        std::cout << "Eintag bleibt Unveraendert" << std::endl;
                        next = true;
                    } else {
                        gorceries.at(itemIndex).amount.decrease(decreaseAm);
                        next = true;
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
                           std::cin >> confirmAdd;
                        } while (confirmAdd == 'Y' || confirmAdd == 'y' || confirmAdd == 'n' || confirmAdd == 'N');
                        if(confirmAdd == 'Y' || confirmAdd == 'N') {
                            prodcuts.at(itemIndex).amount.increase(-decreaseAm);
                        }
                    } else if(decreaseAm == max){
                        std::cout << "Gegenstand komplett veraucht." << std::endl;
                        prodcuts.erase(prodcuts.begin() + itemIndex);
                        next = true;
                    } else if(decreaseAm == 0) {
                        std::cout << "Eintag bleibt Unveraendert" << std::endl;
                        next = true;
                    } else {
                        prodcuts.at(itemIndex).amount.decrease(decreaseAm);
                        next = true;
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
        //[x] Namenvalidierung auf . und existierende Files
        std::fstream file(fileName + ".txt", std::ios::in);
        if(file) {
            nameValid = false;
            std::cout << "Datei existiert bereits!"<<std::endl;
        } else {
            nameValid = true;
        }
        file.close();
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
        for (int i = 0; i < gorceries.size(); i++) {
            fout << "Produkt: " << gorceries.at(i).getName() << std::endl;
            fout << "Anzahl: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Haltbar bis: " << gorceries.at(i).getExpireDateString() << std::endl;
            fout << std::endl;
        }
    }
    fout.close();
    std::cout << "Datei: " << fileName << ".txt wurde geschrieben" << std::endl; 
};

void insertFromFile() {
    std::string dummy;
    std::cout << "Bitte Datei in Hauptordner einfuegen." << std::endl << "Weiter";
    std::cin >> dummy; //Warten Falls Datei noch eingefügt werden muss
    bool nameValid = false;
    std::string fileName;
    do {
        std::cout << "Geben Sie einen Dateinamen an, dateiendung nicht Vergessen!" << std::endl;
        std::cin >> fileName;
        //[x] Namenvalidierung auf . und existierende Files
        std::fstream file(fileName, std::ios::in);
        if(file) {
            nameValid = true;
        } else {
            nameValid = true;
            std::cout << "Datei existiert nicht" <<std::endl;
        }
        file.close();
    } while (!nameValid);

    //Aktuelle Daten können überschrieben werden, und mit den aktuellen ersetzt werden. Dafür doppelte Bestätigung notwendig.
    bool overwrite = false;
    char overwriteAsk;
    char overwriteConfirm;
    bool next = false;
    do {
        std::cout << "Wollen Sie Alle aktuellen Datein überschreiben? [Y/N]" << std::endl;
        std::cin >> overwriteAsk;
        if(overwriteAsk == 'n' || overwriteAsk == 'N') {
            next = true;
        } else if(overwriteAsk == 'y' || overwriteAsk == 'Y') {
            std::cout << "Sind Sie sich sicher? [Y/N]" << std::endl;
            std::cin >> overwriteConfirm;
            if(overwriteConfirm == 'y' || overwriteConfirm == 'Y') {
                next = true;
                overwrite = true;
            }
        }
    } while (!next);
    readFromFile(fileName, !overwrite);
}