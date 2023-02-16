#pragma once
#include"Amount.hpp"
#include<string>

class Signal {
    public:
        /**
         * Diese Methode ruft den Kontruktor der Signalmängen auf um sie im anschluss zu speichern und mit Produkten zu vergleichen. Benötigt den Namen des gegenstandes und die mänge bei der eine meldung geschehen soll.
         * @brief erstellt neue Signalmänge
        */
        Signal(std::string, int);

        /**
         * Durch den aufruf der get Methode der amount klasse gib diese Methode den meldebestand des Objektes als Integer zurück.
         * @brief Gibt den Meldebestand des Objektes als int zurück
         * @return Meldebestand des Objekt als int
        */
        int getSignalAmount();

        /**
         * Durch diese Methode wird der gespeicherte name der Signalmänge zurückgegeben. Dies geschiet im std::string format
         * @brief erhalte namen des Objektes im std::string format
         * @return name des Objekt als std::string
        */
        std::string getName();
        
    private:
        std::string name;
        Amount amount;
};