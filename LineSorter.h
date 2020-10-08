#include <string>
#include <vector>
#include <fstream>

class LineSorter {
public:

    struct Line {
        //The file with Onegin strings should be should be encoded in uit-16 little endian.
        //File should have LF line separators
        char16_t *begin, *end;

        Line(char16_t *begin, char16_t *end);

        Line();

        bool operator<(const Line &another);
    };

private:
    //The file with Onegin strings should be should be encoded in uit-16 little endian.
    //File should have LF line separators
    char16_t *begin = nullptr;
    std::size_t length = 0;
    int fd = -1;

    std::vector<Line> lines;

    void formLines();

public:

    LineSorter(char *filename);

    ~LineSorter();

    void sortLines();

    void printLines(std::basic_ofstream<char16_t> &stream);

};


