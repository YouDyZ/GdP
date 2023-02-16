#pragma once

class Amount {
    public:
        /**
         * Ruf den konstruktor der klasse Amount auf. Klasse ist dafür genutzt den integer wert etwas zu erweitern.
         * @brief erstllt neues Amount objekt
         * @param am Integer wert zum initalisieren.
        */
        Amount(int);

        /**
         * Ruft den Standartkontruktor der Klasse Amount auf und initalisiert dessen Zählwert auf 0;
        */
        Amount();

        /**
         * Gibt den aktuellen wert der des Objektes zurück.
         * @brief erhalte int wert des objektes
         * @return Aktuell gespeicherter wert
        */
        int get();

        /**
         * Überschreibt den aktuellen Wert des Objektes mit der Übergebenen Anzahl.
         * @brief überschreibt Zählervariable
         * @param am neuer wert des Objekt
        */
        void set(int);

        /**
         * Erhöht den Wert der Variable um den übergebenen Faktor
         * @brief Erhöht den wert der Zählervariable
         * @param am drauf addierender Betrag
        */
        void increase(int);

        /**
         * Verringert den Wert der Variable um den Übergebenen Faktor. Variable kann in anschluss ins Negative gehen.
         * @brief Veringert die Zählervariable um den Betrag
         * @param am Betrag der abgezogen wird
        */
        void decrease(int);

    private:
        int count;
};