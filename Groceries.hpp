#pragma once
#include"Products.hpp"
#include<chrono>
#include<ctime>
#include<string>

class Gorceries: public Products {
    public:
        /**
         * Diese Methode ruft den Kontruktor der Erweiterung von Produkt auf, Diese Klasse Speichert zusätlich zum namen und der anzahl des Produktes noch das Ablaufdatum in einem std::tm format um.
         * @brief erstelle ein neues Verderbliches objekt
         * @param iname name des Produktes als std::string
         * @param am Anzahl die das Produkt vorhanden ist als int
         * @param expDate Datum an dem das Produkt abläuift - als std::tm Formalt
        */
        Gorceries(std::string, int, std::tm expDate);

        /**
         * Diese Methode wandelt das gespeicherte std::tm format in das dazu gehörige Datum um. Für die Umformung muss dem objekt am monat 1 Addiert und im Jahr 1900 addiert werden.
         * @brief erzäugt ein string im Datumsformat DD.MM.YYYY
         * @return string des Gespeichertem Datum
        */
        std::chrono::system_clock::time_point getExpireDate();

        /**
         * Diese Methode gibt das Gespeicherte Ablaufdatum des Objektes zurück. Rückgabe geschiet als std::chrono::system_clock und ist geeignet dazu sie mit einem anderem Datum zu vergleichen.
         * @brief erhalte datum asl std::chrono::system_clock::time_point um es mit der systemzeit zu vergleichen.
         * @return gespeichertes Datum als std::chrono::system_clock::timepoint
        */
        std::string getExpireDateString();
    private:
        std::tm expireDate;
};
