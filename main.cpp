#include <iostream>
#include "./src/headers/tabela.h"

using namespace std;

int main() {
    tabela vinhos("vinhos", "./vinho.csv");
    tabela uvas("uvas", "./uva.csv");
    tabela pais("pais", "./pais.csv");
}