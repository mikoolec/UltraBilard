#ifndef UPGRADES_H
#define UPGRADES_H
#pragma once
#include<string>
#include<vector>
struct Upgrade {
    int id;
    std::string nazwa;
    std::string opis;
    int cena;
    int rodzaj; // 0 = kij, 1 = bila
    float wartoscModyfikatora; // np. 0.3f dla +30% siły
};

static std::vector<Upgrade> PelnaBazaUlepszen = {
    // Kije (rodzaj 0)
    {101,"Karbonowy Material", "+3 Strzaly w rundzie", 5, 0},
    {102,"Olowiany Material", "+30% sily strzalu", 5, 0},
    {103,"Zloty Grawerunek", "+20% monet za wbicie bili", 5, 0},
    {104,"Diamentowe Fragmenty", "+30% monet za wbicie bili", 5, 0},
    {105,"Kij Bankiera", "+1 moneta za kazdy strzal", 5, 0},
    {106,"Kij Krolewski", "2x mnoznik punktow", 5, 0},
    {107,"Kij Bilardzisty", "Biala bila nie wytraca predkosci po odbiciu od bandy", 5, 0},
    {108,"Kij Cierpliwosci", "+10 strzalolw w rundzie\n-50% sily strzalu", 5, 0},
    {109,"Kij Pacyfisty", "+5 monet za kazdy pozostaly strzal", 5, 0},
    {110,"Kij Ryzyka", "+50% punktow w rundzie\n3 strzaly na runde", 5, 0},
    {111,"Kij Chaosu", "25% szansy na podwojne monety uderzajac z maksymalna sila", 5, 0},
    {112,"Kij Szczescia", "10% na potrojne punkty\n10% szans na wyzerowanie punktow z rundy", 5, 0},
    {113,"Drewniany Kij", "+100% punktow w rundzie\n5% szans na zlamanie kija i przegranie gry", 5, 0},
    {114,"Kij Faulujacy", "20% szans na +10 monet przy wbiciu bialej bili", 5, 0},
    {115,"Kij Wampira", "+1 strzal za kazda wbita bile", 5, 0},
    {116,"Kij Ducha", "0,5s przenikania bialej bili przez inne bile", 5, 0},
    // Bile (rodzaj 1)
    {201,"Kowadlo", "+10% wielkosci bili\n+10% masy bili", 5, 1},
    {202,"Balon", "-10% wielkosci bili\n-10% masy bili", 5, 1},
    {203,"Kamien", "Biala bila zatrzymuje sie po zderzeniu", 5, 1},
    {204,"Bomba", "Przy pierwszym uderzeniu w bile, odpycha pobliskie bile", 5, 1},
    {205,"Sprezyna", "+10% odbicia od bandy", 5, 1},
    {206,"Kostka Lodu", "Zerowe tarcie bili", 5, 1},
    {207,"Klej", "Natychmiastowe zatrzymanie zaraz po uderzeniu bili", 5, 1},
    {208,"Magnes", "+10% przyciagania do dziury", 5, 1},
    {209,"Duch", "Przenika przez inne bile", 5, 1},
    {210,"Ogien", "+5 punktow za zderzenie z inna bila", 5, 1},
    {211,"Diament", "+5 punktow za kazde odbicie od bandy", 5, 1},
    {212,"Gwiazda", "2x mnoznik punktow bili", 5, 1},
    {213,"Cel", "5x mnoznik punktow bili, jesli zostanie wbita jako pierwsza", 5, 1},
    {214,"Korona", "+10 monet za wbicie bili", 5, 1},
    {215,"Skarbonka", "+5 monet za uderzenie w bile", 5, 1},
    {216,"Inwestycja", "0 punktow za wbicie bili\n2x wszystkie zarobione monety w rundzie", 5, 1},
    {217,"Szkło", "3x posiadane punkty w rundzie\n25% szansa na pekniecie i utrate bili", 5, 1},
};

#endif // UPGRADES_H
