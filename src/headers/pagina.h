#include <vector>
#include <string>
#ifndef PAGINA_H
#define PAGINA_H

class pagina
{
public:
    int colNum;
    std::string pageName;
    static void makePage(int colNum, std::string tableName, std::string pageNumber);
    void writeLine(std::string line);
    std::string getLine(int number);
    void loadPage(std::string filePath);
    pagina();
    ~pagina();
};

#endif