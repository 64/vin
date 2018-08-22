#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <array>

// For mmaping the file
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

struct Span
{
    Span(const char* _start, std::size_t _length, bool _original)
        : start(_start), length(_length), original(_original) {}
    const char* start;
    std::size_t length;
    bool        original;
};

class Sequence
{
public:
    Sequence(const std::string& file_name);
    ~Sequence();

    void insert(std::size_t index, const std::string& text);
    void remove(std::size_t from, std::size_t to);
    char get_ch(std::size_t index);
    const std::list<Span>& pieces();
    void print();

private:
    const char* append(const std::string& text);

private:
    std::list<Span>         chain;
    const char*             original;
    std::array<char, 4096>  modify;
    std::size_t             offset;
    std::size_t             size;

};

#endif // PIECETABLE_H
