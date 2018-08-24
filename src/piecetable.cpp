#include "piecetable.h"

Sequence::Sequence(const std::string& file_name)
    : chain(), offset(0), active(nullptr)
{
    int fd = open(file_name.c_str(), O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    size = sb.st_size;
    original = static_cast<const char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));

    chain.emplace_back(nullptr, 0, 0); // Dummy span
    chain.emplace_back(original, static_cast<size_t>(sb.st_size), true);
    chain.emplace_back(nullptr, 0, 0); // Dummy span
}

Sequence::~Sequence()
{
    // Nothing to do
}

void Sequence::insert_char(std::size_t index, char ch)
{
    // Find span that the index corresponds to
    std::size_t total = 0;
    auto span = get_span(index, total);

    // Add text to modify buffer and retrieve pointer to it
    const char* start = &modify[offset];
    modify[offset++] = ch;

    // Handle Boundry Insertion
    if (total + span->length == index)
    {
        active = &*chain.insert(++span, {start, 1, false});
        return;
    }

    std::size_t length = span->length - (total + span->length - index);

    Span one   {span->start, length, span->original};
    Span two   {start, 1, false};
    Span three {span->start + length, span->length - length, span->original};

    auto it = chain.erase(span);

    // Insert new spans
    it = chain.insert(it, three);
    it = chain.insert(it, two);

    // Avoid inserting empty span
    if (length)
        chain.insert(it, one);

    active = &*it;
}

void Sequence::insert_text(std::size_t index, const std::string& text)
{
    // Find span that the index corresponds to
    std::size_t total = 0;
    auto span = get_span(index, total);

    // Add text to modify buffer and retrieve pointer to it
    const char* start = append_text(text);

    // Handle Boundry Insertion
    if (total + span->length == index)
    {
        chain.insert(++span, {start, text.length(), false});
        return;
    }

    std::size_t length = span->length - (total + span->length - index);

    Span one   {span->start, length, span->original};
    Span two   {start, text.size(), false};
    Span three {span->start + length, span->length - length, span->original};

    auto it = chain.erase(span);

    // Insert new spans
    it = chain.insert(it, three);
    it = chain.insert(it, two);
         chain.insert(it, one);
}

const char* Sequence::append_text(const std::string& text)
{
    for (const auto ch : text)
        modify[offset++] = ch;

    return &modify[offset - text.size()];
}

void Sequence::remove_char(std::size_t index, bool cont)
{
    std::size_t total;
    auto active = get_span(index, total);
    if (total + active->length - 1 == index)
    {
        if (--active->length || active == chain.begin())
            return;
        else
            chain.erase(active);
    }
    else
    {
        std::size_t length = active->length - (total + active->length - index);
        Span one = {active->start, length, active->original};
        Span two = {active->start + length + 1, active->length - length - 1, active->original };
        auto it = chain.erase(active);
        it = chain.insert(it, two);

        if (one.length)
            chain.insert(it, one);
    }
}

char Sequence::get_ch(std::size_t index)
{
    std::size_t total = 0;
    auto it = get_span(index, total);
    std::size_t length = it->length - (total + it->length - index);
    return size ? it->start[length] : 0;
}

std::list<Span>::iterator Sequence::get_span(std::size_t index, std::size_t& total)
{
    total = 0;
    for (auto it = chain.begin(); it != chain.end(); ++it)
        if (index >= total && index < total + it->length)
            return it;
        else
            total += it->length;

    return chain.begin();
}

const std::list<Span>& Sequence::pieces()
{
    return chain;
}

const char* Sequence::start()
{
    return std::next(chain.begin())->start;
}

void Sequence::append_char(char ch)
{
    active->length += 1;
    modify[offset++] = ch;
}

void Sequence::print()
{
//    for (const auto& span : chain)
//    {
//        for (size_t i = 0; i < span.length; ++i)
//            std::cout << span.start[i];
//    }

    for (const auto& span : chain)
    {
        std::cout << "[" << span.length << "]" << " ";
    }

    std::cout << '\n' << std::endl;
}
