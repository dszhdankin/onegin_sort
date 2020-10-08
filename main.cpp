#include <iostream>
#include <fstream>
#include "LineSorter.h"
#include <locale>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Specify filename!\n";
        return 0;
    }

    try {
        LineSorter sorter(argv[1]);
        sorter.sortLines();
        std::basic_ofstream<char16_t> stream;
        stream.open("evgeniy_onegin_sorted.txt");
        sorter.printLines(stream);
        stream.close();
    } catch (std::exception &e) {
        std::cerr << "Cannot open file\n";
    }

    return 0;
}
