#include <iostream>
#include "./src/headers/tabela.h"
#include "./src/classes/sort_externo.cpp"
#include "./src/classes/sort_merge_join.cpp"
using namespace std;

int main() {
    tabela vinhos("vinhos", "./vinho.csv");
    tabela uvas("uvas", "./uva.csv");
    tabela pais("pais", "./pais.csv");
    external_sort("uva", "./uva.csv", 0);       // ordenar por uva_id
    external_sort("vinho", "./vinho.csv", 3);   // ordenar por uva_id
    // Realiza junção uva.uva_id = vinho.uva_id
    sort_merge_join("uva", 0, 1, "vinho", 3, 3, "resultado_join.csv");

}