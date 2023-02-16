#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<chrono>
#include<ctime>
#include<algorithm>
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
void reWriteSignalAsNew();
void exit();
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
    exit();
    return 0;
};

/**
 * Simple funktion im das Loggen und damit das Debuggen damit erleichern soll
 * @brief gibt ein string im std::cout aus.
 * @param message String der ausgegeben werden soll
 * @param nl true, wenn im anschluss eine Neue Zeile erstellt werden soll, default: false;
*/
void log(std::string message, bool nl = false) {
    std::cout << message;
    if(nl) {
        std::cout << std::endl;
    }
}

/**
 * Liest von der Übergebenen Datei die Produkte ein und fügt Sie in den Gorcheries und Products Vektor ein.
 * @brief Liest von Datei ein und sotiert in Gorceries und Products vector.
 * @details List aus der übergebenen, im Hauptordner zu befindende Datei ein und untrerscheidet diese im Anschluss anhand des Ablaufdatums ob diese in Ein Verderbliches Produkt sind oder nicht. Sollte es kein Verderbliches Produkt sein wird dies anhand des Fehlenden Ablaufdatum oder Ablaufdatum: keines Erkannt. 
 * Diese Methode benötigt eine Datei mit der Syntax: Name: [BEZECHNER] \n Menge: [INT] \n Ablaufdatum: [keines|DD.MM.YYYY] *optional
 * Bei den Objeten wird auf Dopplung, anhand des Name, überprüft, sollte diesen Vorhanden sein bei nicht verderblichen Produkten wird dies aufeinander Addiert, bei Verderblichen gegenständen muss dazu noch das Ablaufdatum übereinstimmen.
 * Sollte Keine Meldemänge vorhanden sein für dein eingelesenes Produkt, so wird gefragt, welche mänge Verwendet werden soll mit der Option im anschluss die singal.txt, welche als Datenbank für die Meldemängen besteht, neu zu generieren mit allen Aktuell gespeicherten Signalmängen.
*/
void readFromFile(std::string fileName, bool append = false) {
    if(!append) {
        prodcuts.clear();
        gorceries.clear();
    }
    std::ifstream file(fileName);
    std::string fileLine;

    std::string itemName;
    std::string expDate;
    bool expIsProd = false;
    int amount;

    bool reWriteSignal = false;

    while (std::getline(file, fileLine)) {
        std::stringstream sstream(fileLine);
        std::string valueType, value;
        sstream >> valueType >> value;

        if(value != "") {
            if(valueType == "Name:") {
                itemName = value;
            } else if(valueType == "Ablaufdatum:") {
                if(value == "keines") {
                    expIsProd = true;
                    expDate = "";
                } else if(value == "") {
                    expIsProd = true;
                    expDate = "";
                } else {
                    expDate = value;
                }
                
            } else if(valueType == "Menge:") {
                amount = stoi(value);
            } else {
                std::cout << "Fehler in der Datei. Bitte Datei: '" << fileName << "' überprüfen!" << std::endl;
                return;
            }
        } else {
            bool hasSignalAmount = false;
            if (signalAmounts.size() > 0) {
                for (long unsigned int i = 0; i < signalAmounts.size(); i++) {
                    if(itemName == signalAmounts.at(i).getName()) {
                        hasSignalAmount = true;
                    }
                }
            }
            
            int setSignalAmount;
            if(!hasSignalAmount) {
                do {
                    std::cout << "Bitte geben Sie ein Meldebestand für " << itemName << " ein!" << std::endl;
                    std::cin >> setSignalAmount;
                    if(!reWriteSignal) {
                        bool confirmSignal = false;
                        char inConfirmSignal;
                        do {
                            std::cout << "Möchten Sie die Signal.txt neu erzeugen? [Y/N]" << std::endl;
                            std::cin >> inConfirmSignal;
                            if(inConfirmSignal == 'y' || inConfirmSignal == 'Y') {
                                reWriteSignal = true;
                                confirmSignal = true;
                            } else if(inConfirmSignal == 'n' || inConfirmSignal == 'N') {
                                confirmSignal = true;
                            }
                        } while (!confirmSignal);
                    }
                } while(setSignalAmount < 0);
                Signal tempSignl = Signal(itemName, setSignalAmount);
                signalAmounts.push_back(tempSignl);
            }

            if(!expIsProd) {
                //ist ein Gorcerie
                int expDateDay, expDateMonth,expDateYear;
                char dummy1, dummy2;

                std::stringstream sstream(expDate);
                sstream >> expDateDay >> dummy1 >> expDateMonth >> dummy2 >> expDateYear;
                std::tm expDatetm = {0,0,0, expDateDay, expDateMonth-1, expDateYear-1900};
                //gegenstand bereits vorhanden.

                bool added = false;
                
                for(long unsigned int k = 0; k < gorceries.size(); k++) {
                    if(gorceries.at(k).getName() == itemName) {
                        if(gorceries.at(k).getExpireDate() == std::chrono::system_clock::from_time_t(std::mktime({&expDatetm}))) {
                            if(!added) {
                                gorceries.at(k).amount.increase(amount);
                                added = true;
                            }
                        }
                    }
                } 
                if(!added) {
                    gorceries.push_back(Gorceries(itemName, amount, expDatetm));
                }
                
            } else {
                if(prodcuts.size() != 0) {
                    bool added = false;
                    for(long unsigned int k = 0; k < prodcuts.size(); k++) {
                        if(prodcuts.at(k).getName() == itemName) {
                            
                            if(!added) {
                                prodcuts.at(k).amount.increase(amount);
                                added = true;
                            }
                        }
                    }
                    if(!added) {
                        prodcuts.push_back(Products(itemName, amount));
                    }
                } else {
                    prodcuts.push_back(Products(itemName, amount));
                }
                
            }
            //reset for next loop
            itemName = "";
            expDate = "";
            amount = 0;
            expIsProd = false;
        }
    }
    if(reWriteSignal) {
        reWriteSignalAsNew();
    }
}

/**
 * Initialaufruf für das Programm, aktualisiert dabei die Datein, bei denen der Bestand niedrig ist und welches bald abläuft. Setzt das aktuelle Datum
 * @brief Initalisiert das Programm
 */
void init() {
    std::cout << "Signalwerte werden gelesen" << std::endl;
    readSignalAmount();
    std::cout << "Daten werden gelesen aus data.txt" << std::endl;
    readFromFile("data.txt");

    std::cout << "Liste mit bald zu essenden Produkten wird aktuallisiert" << std::endl;
    
    today = std::chrono::system_clock::now();
    checkToEat();

    std::cout << "Datei wird aktuallisiert, mit pordukten unter Signalanzahl" << std::endl;
    checkAmountLow();
};

/**
 * Funktion zum Initialen aufrufen der Signalmengen. Mit überprüfung aus Dopplung und und der möglichkeit bei Dopplung den wert, der behalten werden soll auszuwählen.
 * @brief Funktion zum initialisieren der signalmengen
 * @pre signal.txt im haupordner mit stuktur: [ITEMNAME] [SIGNALMENGE] \n
 * @details Die Funktion ist für den Initialaufruf der signalmengen gedacht, wärend sie läuft pusht sie alle Signalmengen als in den vector Signals. Sollte eine Dopplung mit gleichem wert vorhanden sein, kommt eine meldung und die option die signal.txt erneut zu generieren, Am ende der funktion. Bei dopplung mit unterschiedlichen werten, wurd ein wert davon durch die nutzereingabe 1 oder 2 ausgeähl für das jeweilige objekt. ebenfalls die option danach die signal.txt erneut zu erzeugen. Sollte dem einmal zugestimmt wurden sein, taucht die meldung nicht mehr auf. 
*/
void readSignalAmount() {
    std::ifstream file("singal.txt");
    std::string fileLine;
    bool reWriteFile = false;
    std::vector<int> removeAfter;
    while (std::getline(file, fileLine)) {
        std::stringstream sstream(fileLine);
        std::string valueName, value;
        sstream >> valueName >> value;;
        if(signalAmounts.size() == 0) {
            signalAmounts.push_back(Signal(valueName, std::stoi(value)));
        } else {
            //Dopplung Überpüfen
            bool added = false;
            for (long unsigned int i = 0; i < signalAmounts.size(); i++) {
                if ((signalAmounts.at(i).getName() == valueName) && (signalAmounts.at(i).getSignalAmount() == std::stoi(value))) {
                    std::cout << "Dopplung bei den Signalwerten vorhanden. - Bitte Dopplung enfernen um diese Meldung zu deaktivieren." << std::endl <<  "Item name: " << valueName << std::endl;
                    if(!reWriteFile) {
                        char reWrite;
                        bool goNext = false;
                        do {
                            std::cout << "Wollen Sie die Datei im Anschluss neu erzeugen? [Y/N]" << std::endl;
                            std::cin >> reWrite;
                            if(reWrite == 'Y' || reWrite == 'y') {
                                reWriteFile = true;
                                goNext = true;
                            } else if(reWrite == 'N' || reWrite == 'n') {
                                goNext = true;
                            }
                        } while (!goNext);
                    }
                } else if(signalAmounts.at(i).getName() == valueName) {
                    int settedSignal;
                    do {
                        std::cout << "Dopplung bei den Signalwerten vorhanden. - Unterschiedliche Signalwerte. Bitte auswählen" << std::endl;
                        std::cout << valueName << std::endl;
                        std::cout << "1: " << value << std::endl;
                        std::cout << "2: " << signalAmounts.at(i).getSignalAmount() << std::endl;
                        std::cin >> settedSignal;
                    } while (!(settedSignal == 1 || settedSignal == 2));
                    if(settedSignal == 1) {
                        removeAfter.push_back(i);
                        signalAmounts.push_back(Signal(valueName, std::stoi(value)));
                        added = true;
                    } 
                    if(added) {
                        //die zu entfernenden sortieren, von groß nach klein
                        std::sort(removeAfter.begin(), removeAfter.end(), std::greater<int>());
                        //Dopplung enfternen
                        std::vector<int>::iterator last = std::unique(removeAfter.begin(), removeAfter.end());
                        removeAfter.erase(last, removeAfter.end());
                        
                        //von uprünglichem vector enfertnen;
                        for(long unsigned int k = 0; k < removeAfter.size(); k++) {
                           signalAmounts.erase(signalAmounts.begin() + removeAfter.at(k)); 
                        }
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
                } 
            }
            if (!added) {
                signalAmounts.push_back(Signal(valueName, std::stoi(value)));
            }
        }
    }
    if(reWriteFile) {
        reWriteSignalAsNew();
    }
};

/**
 * Erzeugt eine neue signal.txt mit den aktuellen inhalten des Vektors SignalAmounts
 * @brief erstellt neue signal.txt
*/
void reWriteSignalAsNew() {
    std::cout << "signal.txt wird neu erzeugt..." << std::endl;
    remove("singal.txt");
    std::ofstream fout;
    fout.open("singal.txt");
    for (long unsigned int i = 0; i < signalAmounts.size(); i++) {
        fout << signalAmounts.at(i).getName() << " " << signalAmounts.at(i).getSignalAmount() << std::endl;
    }
    fout.close();
    std::cout << "signal.txt erfolgreich erneut erzeugt" << std::endl;
}

/**
 * Erzeugt eine Datei mit den Verderblichen gegenständen, die in den nächsten 7 Tagen abläuft.
 * @brief erstelt baldEssen.txt 
 * @pre today mit aktuellem datum
*/
void checkToEat() {
    std::vector<Gorceries> nearExpDate;
    std::chrono::system_clock::time_point nowInSevenDays = today + std::chrono::seconds(7*24*60*60);
    for (long unsigned int i = 0; i < gorceries.size(); i++) {
        Gorceries prod = gorceries.at(i);
        if(prod.getExpireDate() > nowInSevenDays) {
            nearExpDate.push_back(prod);
        }
    }
    std::cout << "Erstelle Datei mit bald verderbenden Produkten..." << std::endl;

    std::ofstream fout;
    fout.open("baldEssen.txt");
    fout << "--Bald zu essende Produkte--" << std::endl;
    for (long unsigned int i = 0; i < nearExpDate.size(); i++) {
        fout << "Name: " << nearExpDate.at(i).getName() << std::endl;
        fout << "Menge: " << nearExpDate.at(i).amount.get() << std::endl;
        fout << "Ablaufdatum: " << nearExpDate.at(i).getExpireDateString() << std::endl;
    }
    fout.close();
    std::cout << "Datei mit Bald zu essenden Produkten als 'baldEssen.txt' gespeichert!" << std::endl;
};

/**
 * Überprüft die Verderblichen und unverderblichen Produkte in vergleich mit dem Meldebestand der signalAmouts Vector und genertiert darauf basierend eine Datei "nachkaufen.txt" mit den Produkten, die verbaucht werden müssen
 * @brief schreibt produkte niedriger als meldebstand in nachkaufen.txt
*/
void checkAmountLow() {
    std::ofstream fout;
    fout.open("nachkaufen.txt");
    if(prodcuts.size() > 0) {
        fout << "--Unerderbliche Produkte--" << std::endl;
    }
    for (long unsigned int i = 0; i < prodcuts.size(); i++) {
        int signalAm;
        for(long unsigned int k = 0; k < signalAmounts.size(); k++) {
            if(signalAmounts.at(k).getName() == prodcuts.at(i).getName()) {
                signalAm = signalAmounts.at(k).getSignalAmount();
            }
        }
        if(prodcuts.at(i).amount.get() < signalAm) {
            fout << "Name: " << prodcuts.at(i).getName() << std::endl;
            fout << "Menge: " << prodcuts.at(i).amount.get() << std::endl;
            fout << "" << std::endl;
        }
    }
    //erzeugt leere extra zeile für schönere Trennung 
    if((prodcuts.size() > 0) && (gorceries.size() > 0)) {
        fout << "" << std::endl;
    }
    if(gorceries.size() > 0) {
        fout << "--Verderbliche Produkte--" << std::endl;
    }
    for (long unsigned int i = 0; i < gorceries.size(); i++) {
        int signalAm;
        for(long unsigned int k = 0; k < signalAmounts.size(); k++) {
            if(signalAmounts.at(k).getName() == gorceries.at(i).getName()) {
                signalAm = signalAmounts.at(k).getSignalAmount();
            }
        }
        if(gorceries.at(i).amount.get() < signalAm) {
            fout << "Name: " << gorceries.at(i).getName() << std::endl;
            fout << "Menge: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Ablaufdatum: " << gorceries.at(i).getExpireDateString() << std::endl;
            fout << "" << std::endl;
        }
    }
    fout.close();
    std::cout << "Produkte mit niedrigem Bestand in nachkaufen.txt geschrieben" << std::endl;
}

/**
 * erstellt das hauptmenu und nimmt den Input für den ausgeählten punkt. Überprüft ob der ausgeählt punkt teil des menüs ist.
 * @brief erstelle hauptmenue
 * @return int des menuepunkt
*/
int menue() {
    int input;
    do {
        log("--------------------------------", true);
        log("1: Neues Item einfuegen", true);
        log("2: Items verbrauchen", true);
        log("3: Items in Datei Speichern", true);
        log("4: Items aus Datei einlesen", true);
        log("0: Exit", true);
        log("--------------------------------", true);
        std::cin >> input;
    } while (input < 0 || input > 4);
    return input;
};

/**
 * Starte den Programmablauf zum hinzufügen eines neuen Gegenstandes und fügt dieses dem gorceries oder prodcuts vector zu.
*/
void addNewItem() {
    bool isGrocery = false;
    char input;
    bool exit = false;
    bool next = false;
    do {
        do {
            std::cout << "Ist das Item verderblich? [Y/N]"<<std::endl;
            std::cin >> input;
            if(input == 'Y' || input == 'y' || input == 'n' || input == 'N') {
                next = true;
            };
        } while(!next);
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
                std::cout << "Wann läuft das Produkt ab? "<< std::endl << "Tag: ";
                std::cin >> expDateDay;
                std::cout << "Moant: ";
                std::cin >> expDateMonth;
                std::cout << "Jahr: ";
                std::cin >> expDateYear;
                expDate = {0,0,0, expDateDay, expDateMonth-1, expDateYear-1900};
                std::cout << "Produkt läuft "   << expDate.tm_mday << "." 
                                                << expDate.tm_mon + 1  << "." 
                                                << expDate.tm_year + 1900
                                                << " ab? [Y/N]" <<std::endl;
                std::cin >> in;
                if(in == 'y' || in == 'Y') {
                    confirm = true;
                }
            } while (!confirm);

            //check item doubled
            bool added = false;
            for (long unsigned int k = 0; k < gorceries.size(); k++) {
                if(gorceries.at(k).getName() == prodName) {
                    if(std::chrono::system_clock::from_time_t(std::mktime(&expDate)) == gorceries.at(k).getExpireDate()) { 
                        gorceries.at(k).amount.increase(prodAmount);
                        added = true;
                        std::cout << "Gegenstand mit gleichem Namen und Gleichen Ablaufdatum im system, Gegenstände wurden zusammengeführt" << std::endl;
                    }
                }
            }
            if(!added) {
                gorceries.push_back(Gorceries(prodName, prodAmount, expDate));
            }
            
        } else {
            bool added = false;
            for(long unsigned int k = 0; k < prodcuts.size(); k++) {
                if(prodcuts.at(k).getName() == prodName) {
                    prodcuts.at(k).amount.increase(prodAmount);
                    std::cout<<"gegenstand breits vorhanden. Gelagerter Gegenstand wurde erhöht!" << std::endl;
                }
            }
            if(!added) {
                prodcuts.push_back(Products(prodName, prodAmount));
            }
            
        }
        next = false;
        do {
            std::cout << "Weiteres Item hinzufügen?" <<std::endl;
            std::cin >> input;
            if(input == 'Y' || input == 'y') {
                next = true;
            } else if(input == 'N' || input == 'n') {
                next = true;
                exit = true;
            };
        } while(!next);
    } while (!exit);
};

/**
 * Startet den Programmablauf zum verbrauchen eines Gegenstandes
 * @brief verbraucht einen Gegenstand aus dem Gespeicherten Vectors
 * @details Erfragt den Namen des zu berauchenden Gegenstandes der verbeaucht werden soll. Sollte es ein Verderblicher gegenstand sein mit mehreren einträgen und unterschiedlichen ablaufdatum wird dem nutzer eine liste aller gefunden gegensänden gegeben mit der anzahl die dieser verhanden ist und der nutzer wählt aus welchen gegensant er verbauchen will. Sollte der verbrauch negativ sein, so wird der Verbauch hinzugefügt nach rückfrage des nutzer, sollte er gleich der vorhanden menge sein, wird der gegenstand komplett veraucht und dem vector enfernt.
*/
void useItem() {
    bool confirm = false;
    std::string prodName;
    do {
        char in;
        std::cout << "Wie ist der Name des Produkts?" << std::endl; 
        std::cin >> prodName;
        std::cout << "Ist " << prodName << " richtig? [Y/N]" << std::endl;
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
            for (long unsigned int k = 0; k < resProd.size(); k++) {
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
            for (long unsigned int k = 0; k < resGroc.size(); k++) {
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
        long unsigned int selectItem;
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
                    if(decreaseAm > max)  {
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

/**
 * Legt eine kopie aller aktuellen Daten an und speichert die in einer vom nutzer benannten .txt datei
 * @brief speichert alle Daten in erfragen Dateiname .txt
*/
void saveItems() {
    std::string fileName;
    std::cout << "Datei wird als Textdatei gespeichert!" << std::endl;
    bool nameValid = false;
    do {
        std::cout << "Geben Sie einen Dateinamen an!" << std::endl;
        std::cin >> fileName;
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
        fout << "--Verderbliche Produkte--" << std::endl;
        fout << std::endl;
        for (long unsigned int i = 0; i < gorceries.size(); i++) {
            fout << "Produkt: " << gorceries.at(i).getName() << std::endl;
            fout << "Anzahl: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Haltbar bis: " << gorceries.at(i).getExpireDateString() << std::endl;
            fout << std::endl;
        }
    }
    fout.close();
    std::cout << "Datei: " << fileName << ".txt wurde geschrieben" << std::endl; 
};

/**
 * liest eine datei vom nutzer ein und fügt diese dem aktuellen bestand zu. Datei sollte die gleiche syntax wie date.txt haben
 * @brief Liest datei von nutzer ein und entweder ersersetzt alle vohandenen Daten oder fügt Sie hinzu
*/
void insertFromFile() {
    std::cout << "Bitte Datei in Hauptordner einfuegen." << std::endl;
    bool nameValid = false;
    std::string fileName;
    do {
        std::cout << "Geben Sie einen Dateinamen an, dateiendung nicht Vergessen!" << std::endl;
        std::cin >> fileName;
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

/**
 * Speichert alle aktuellen daten in data.txt für nächste nutzung
 * @brief erzeugt neue data.txt mit aktuellen werten für nache siztung
*/
void exit() {
    remove("data.txt");
    std::ofstream fout;
    fout.open("data.txt");
    if(prodcuts.size() != 0) {
        for (size_t i = 0; i < prodcuts.size(); i++) {
            fout << "Name: " << prodcuts.at(i).getName() << std::endl;
            fout << "Menge: " << prodcuts.at(i).amount.get() << std::endl;
            fout << "Ablaufdatum: keines" << std::endl;
            fout << std::endl;
        };
    }
    if(gorceries.size() != 0) {
        for (long unsigned int i = 0; i < gorceries.size(); i++) {
            fout << "Name: " << gorceries.at(i).getName() << std::endl;
            fout << "Menge: " << gorceries.at(i).amount.get() << std::endl;
            fout << "Ablaufdatum: " << gorceries.at(i).getExpireDateString() << std::endl;
            fout << std::endl;
        }
    }
    fout.close();
}
