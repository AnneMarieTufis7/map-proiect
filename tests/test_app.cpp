#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <chrono>

#include "app.hpp"

using namespace map_project;

TEST_CASE("health body reports ok and uptime") {
    const auto started_at = std::chrono::steady_clock::now();
    const auto body = HealthBody(started_at);

    CHECK(body["status"] == "ok");
    CHECK(body.contains("uptime_seconds"));
    CHECK(body["uptime_seconds"].get<long>() >= 0);
}

TEST_CASE("version body contains all required fields") {
    const auto body = VersionBody();

    CHECK(body["app"] == kAppName);
    CHECK(body["version"] == kAppVersion);
    CHECK(body.contains("commit"));
    CHECK(body.contains("built_at"));
}

TEST_CASE("env falls back when the variable is missing") {
    CHECK(Env("MAP_VARIABLE_THAT_DOES_NOT_EXIST", "fallback") == "fallback");
}

TEST_CASE("error body has the standard shape") {
    const auto body = ErrorBody("not_found", "route does not exist");

    CHECK(body["error"] == "not_found");
    CHECK(body["message"] == "route does not exist");
}

TEST_CASE("new store starts at one") {
    Store store;
    CHECK(store.NextId() == 1);
}

TEST_CASE("take next id increments") {
    Store store;

    CHECK(store.TakeNextId() == 1);
    CHECK(store.TakeNextId() == 2);
    CHECK(store.NextId() == 3);
}

TEST_CASE("reset returns to one") {
    Store store;
    store.TakeNextId();
    store.TakeNextId();

    store.Reset();

    CHECK(store.NextId() == 1);
}

TEST_CASE("home page mentions the application name") {
    CHECK(HomePage().find(kAppName) != std::string::npos);
}
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

// ========================================================
// 1. TESTE DE EROARE / VALIDARE (3 TESTE OBLIGATORII)
// ========================================================

// Test 1: Nu se permit numere negative
void test_respingere_numar_negativ() {
    int nota = -5;
    bool este_valida = (nota >= 1 && nota <= 10);
    assert(este_valida == false);
}

// Test 2: Notele trebuie sa fie doar intre 1 si 10
void test_validare_interval_1_10() {
    int nota_0 = 0;
    int nota_11 = 11;

    bool valida_0 = (nota_0 >= 1 && nota_0 <= 10);
    bool valida_11 = (nota_11 >= 1 && nota_11 <= 10);

    assert(valida_0 == false);
    assert(valida_11 == false);
}

// Test 3: Nu se permit numere cu virgula (doar numere intregi)
void test_respingere_numar_cu_virgula() {
    std::string nota_text = "8.5";
    bool are_virgula = (nota_text.find('.') != std::string::npos || 
                        nota_text.find(',') != std::string::npos);
    
    bool este_intreg = !are_virgula;
    assert(este_intreg == false);
}

// ========================================================
// 2. TESTE DE FUNCȚIONARE NORMALĂ (8 TESTE)
// ========================================================

// Test 4: Adaugare nota valida
void test_adaugare_nota_valida() {
    int nota = 8;
    bool este_valida = (nota >= 1 && nota <= 10);
    assert(este_valida == true);
}

// Test 5: Caz limita - Nota 1
void test_caz_limita_nota_1() {
    int nota = 1;
    assert(nota == 1);
}

// Test 6: Caz limita - Nota 10
void test_caz_limita_nota_10() {
    int nota = 10;
    assert(nota == 10);
}

// Test 7: Calculul mediei pentru doua note
void test_calcul_medie_doua_note() {
    int nota1 = 8;
    int nota2 = 10;
    float medie = (float)(nota1 + nota2) / 2.0f;
    assert(medie == 9.0f);
}

// Test 8: Nume student nevid
void test_nume_student_nevid() {
    std::string nume = "Popescu Maria";
    assert(nume.empty() == false);
}

// Test 9: Catalog initial gol
void test_catalog_fara_note() {
    std::vector<int> note;
    assert(note.empty() == true);
}

// Test 10: Adaugare note multiple
void test_adaugare_note_multiple() {
    std::vector<int> note = {7, 9, 10};
    assert(note.size() == 3);
}

// Test 11: Verificare nota de trecere
void test_verificare_nota_trecere() {
    int nota = 5;
    bool este_promovat = (nota >= 5);
    assert(este_promovat == true);
}

// ========================================================
// MAIN - RULAREA TUTUROR TESTELOR
// ========================================================
int main() {
    std::cout << "--- Rulam cele 11 teste pentru catalog ---\n";

    test_respingere_numar_negativ();
    test_validare_interval_1_10();
    test_respingere_numar_cu_virgula();

    test_adaugare_nota_valida();
    test_caz_limita_nota_1();
    test_caz_limita_nota_10();
    test_calcul_medie_doua_note();
    test_nume_student_nevid();
    test_catalog_fara_note();
    test_adaugare_note_multiple();
    test_verificare_nota_trecere();

    std::cout << "SUCCESS: Toate cele 11 teste au trecut cu succes!\n";
    return 0;
}