#pragma once
#include"Amount.hpp"
#include<string>

class Products {
    protected:
        std::string name;
        
    public:
        /**
         * Hauptknstruktor zum erstellen eines Produkte, erstellt Das neue, nicht verderbliche, Produkt um später dies zu speichern. Die Gepeicherte anzhal basiert auf dem Amount Objektes
         * @brief Erstellet ein neuen gegenstand
         * @param iname Name des Gegenstandes
         * @param am Anzahl des gegenstandes
        */
        Products(std::string, int);

        /**
         * Standartkontruktor notwendig um die Klasse zu erweitern.
         * @brief Kontruktor sollte nicht genutzt werden.
        */
        Products();

        /**
         * Gibt den Namen des Gegenstandes im std::string format zurück
         * @brief erhalte produktname im std::string format
        */
        std::string getName();

        /**
         * Anzahl des gespeicherten Objektes unter verwendung der Amount klasse
        */
        Amount amount;
};

