#include <iostream>
#include <string>

// Clasa de baza Mamifer
class Mamifer {
protected:
    std::string nume;
    int varsta;

public:
    // Constructorul clasei Mamifer
    Mamifer(const std::string& nume, int varsta) 
        : nume(nume), varsta(varsta) {}

    // Metoda de afisare a datelor comune
    void afiseazaMamifer() const {
        std::cout << "Nume: " << nume << "\n"
                  << "Varsta: " << varsta << " ani\n";
    }
};
//
// Clasa derivata Cangur (mostenire publica)
class Cangur : public Mamifer {
    double lungimeSaritura; // atribut propriu

public:
    // Constructorul clasei Cangur care apeleaza constructorul bazei
    Cangur(const std::string& nume, int varsta, double lungimeSaritura)
        : Mamifer(nume, varsta), lungimeSaritura(lungimeSaritura) {}

    // Metoda de afisare specifica pentru Cangur
    void afiseaza() const {
        std::cout << "--- Detalii Cangur ---\n";
        afiseazaMamifer();
        std::cout << "Lungime saritura: " << lungimeSaritura << " metri\n\n";
    }
};

// Clasa derivata Urs (mostenire publica)
class Urs : public Mamifer {
    std::string tipUrs; // atribut propriu

public:
    // Constructorul clasei Urs care apeleaza constructorul bazei
    Urs(const std::string& nume, int varsta, const std::string& tipUrs)
        : Mamifer(nume, varsta), tipUrs(tipUrs) {}

    // Metoda de afisare specifica pentru Urs
    void afiseaza() const {
        std::cout << "--- Detalii Urs ---\n";
        afiseazaMamifer();
        std::cout << "Tip urs: " << tipUrs << "\n\n";
    }
};

int main() {
    // Crearea obiectelor direct in main, fara meniuri sau tablouri
    Cangur cangur("Skippy", 5, 9.5);
    Urs urs("Baloo", 10, "Urs Brun");

    // Afisarea datelor
    cangur.afiseaza();
    urs.afiseaza();

    return 0;
}







