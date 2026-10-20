#include <iostream>
#include <string>

// Clasa de baza Persoana
class Persoana {
protected:
    std::string nume;
    int varsta;

public:
    // Constructorul clasei Persoana
    Persoana(const std::string& nume, int varsta) 
        : nume(nume), varsta(varsta) {}

    // Metoda de afisare a datelor comune
    void afiseazaPersoana() const {
        std::cout << "Nume: " << nume << "\n"
                  << "Varsta: " << varsta << " ani\n";
    }
};

// Clasa derivata Student (mostenire publica)
class Student : public Persoana {
    std::string facultate;
    int anStudiu;

public:
    // Constructorul clasei Student care apeleaza constructorul bazei
    Student(const std::string& nume, int varsta, const std::string& facultate, int anStudiu)
        : Persoana(nume, varsta), facultate(facultate), anStudiu(anStudiu) {}

    // Metoda de afisare care combina datele comune si cele specifice
    void afiseazaStudent() const {
        std::cout << "--- Detalii Student ---\n";
        afiseazaPersoana(); // Afiseaza datele comune din Persoana
        std::cout << "Facultate: " << facultate << "\n"
                  << "An de studiu: " << anStudiu << "\n\n";
    }
};

int main() {
    // Crearea a doi studenti diferiti direct in main
    Student student1("Tufis Anne-Marie", 20, "Automatica si Calculatoare", 2);
    Student student2("Alexandru Popescu", 21, "Automatica si Calculatoare", 3);

    // Afisarea datelor comune si specifice pentru ambii studenti
    student1.afiseazaStudent();
    student2.afiseazaStudent();

    return 0;
}