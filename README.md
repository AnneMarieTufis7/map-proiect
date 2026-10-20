[![CI](https://github.com/AnneMarieTufis7/map-proiect/actions/workflows/ci.yml/badge.svg)](https://github.com/AnneMarieTufis7/map-proiect/actions/workflows/ci.yml)
# [Titlul proiectului]

Proiect individual la disciplina Metode avansate de programare, anul universitar 2026-2027.

## Autor

- **Nume:** Tufis Anne Marie
- **Grupa:** 2.2
- **Marca:** 715715
- **Tema:** 3 - catalog de note

## Descriere

Proiectul constă în dezvoltarea unui serviciu web care digitalizează operațiunile clasice ale unui catalog școlar.  Scopul principal al aplicației este de a menține evidența studenților, a notelor obținute de aceștia și de a calcula automat situațiile școlare pe baza datelor introduse.

## Tehnologii

C++20 cu cpp-httplib si nlohmann/json

## Rulare

```
docker build -t map-proiect .
docker run -d -p 8080:8080 map-proiect
```

Aplicatia asculta pe portul 8080. Verificati:

```
curl http://localhost:8080/health
curl http://localhost:8080/version
```

## Testare

```
cmake -B build -DBUILD_TESTS=ON
cmake --build build -j 
./build/tests
```

## Rutele implementate

| Ruta | Metoda | Descriere |
|---|---|---|
| `/health` | GET | Starea serviciului |
| `/version` | GET | Versiunea si commit-ul din care a fost construita imaginea |
| `/` | GET | Pagina de prezentare |
| `/reset` | POST | Goleste datele din memorie |
| [ruta temei] | [metoda] | [descriere] |

## Decizii de implementare

[Doua-trei decizii tehnice pe care le-ati luat si motivul fiecareia.]