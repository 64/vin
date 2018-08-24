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

    void insert_char(std::size_t index, char ch);
    void insert_text(std::size_t index, const std::string& text);
    const char* append_text(const std::string& text);
    void append_char(char ch);
    void remove_char(std::size_t index, bool cont);
    char get_ch(std::size_t index);
    void print();
    const char* start();
    std::list<Span>::iterator get_span(std::size_t index, std::size_t& total);
    const std::list<Span>&    pieces();

private:

private:
    std::list<Span>         chain;
    const char*             original;
    std::array<char, 4096>  modify;
    std::size_t             offset;
    std::size_t             size;
    Span*                   active;

};

#endif // PIECETABLE_H
