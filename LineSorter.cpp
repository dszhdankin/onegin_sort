#include "LineSorter.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <algorithm>
#include <iostream>
#include <locale>

LineSorter::LineSorter(char *filename) {
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        throw std::exception();
    }

    struct stat metadata;
    fstat(fd, &metadata);

    length = metadata.st_size / 2;
    begin = (char16_t *) mmap(NULL, metadata.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    formLines();
}

LineSorter::Line nextLine(char16_t *&ptr, char16_t *end) {
    LineSorter::Line line;
    line.begin = ptr;
    while (ptr < end && *ptr != 10) {
        ptr++;
    }
    line.end = ptr;
    ptr++;
    return line;
}

void LineSorter::formLines() {
    int cnt = 0;

    for (char16_t *ptr = begin; ptr < begin + length; ptr++) {
        if (*ptr == 10)
            cnt++;
    }
    lines.reserve(cnt + 2);

    char16_t *ptr = begin + 1;
    while (ptr < begin + length) {
        lines.push_back(nextLine(ptr, begin + length));
    }
}

LineSorter::~LineSorter() {
    close(fd);
    munmap(begin, length*2);
}

LineSorter::Line::Line(char16_t *begin, char16_t *end) {
    this->begin = begin;
    this->end = end;
}

LineSorter::Line::Line() {
    this->begin = this->end = nullptr;
}


char16_t *nextLetter(const LineSorter::Line &line, char16_t *&pos, std::locale &loc) {
    pos--;
    while (pos >= line.begin && !isalpha((wchar_t ) *pos, loc)) {
        pos--;
    }
    return pos;
}

bool LineSorter::Line::operator<(const Line &another) {
    std::locale loc("en_US.UTF8");
    char16_t *curLeft = this->end, *curRight = another.end;
    while (nextLetter(*this, curLeft, loc) >= this->begin &
            nextLetter(another, curRight, loc) >= another.begin) {
        if (*curLeft < *curRight)
            return true;
        else if (*curLeft > *curRight)
            return false;
    }
    if (curLeft < this->begin && curRight >= another.begin)
        return true;
    else
        return false;
}

void LineSorter::sortLines() {
    std::sort(lines.begin(), lines.end());
}

void printLine(LineSorter::Line line, std::basic_ofstream<char16_t> &stream) {
    for (char16_t *ptr = line.begin; ptr < line.end; ptr++)
        stream.put(*ptr);
    stream.put((char16_t )'\n');
}

void LineSorter::printLines(std::basic_ofstream<char16_t> &stream) {
    for (int i = 0; i < lines.size(); i++) {
        printLine(lines[i], stream);
    }
}
