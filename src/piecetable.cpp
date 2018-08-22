#include "piecetable.h"

Sequence::Sequence(const std::string& file_name) : chain(), offset(0)
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

void Sequence::insert(std::size_t index, const std::string& text)
{
    std::size_t total = 0;
    auto span = chain.begin();

    // Find span that the index corresponds to
    for (auto it = chain.begin(); it != chain.end(); ++it)
    {
        if (index >= total && index <= total + it->length)
        {
            span = it;
            break;
        }

        total += it->length;
    }

    // Add text to modify buffer and retrieve pointer to it
    const char* start = append(text);

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

void Sequence::remove(std::size_t from, std::size_t to)
{

}

char Sequence::get_ch(std::size_t index)
{
    int total = 0;
    for (auto it = std::next(chain.begin()); it != chain.end(); ++it)
    {
        if (index >= total && index <= total + it->length)
        {
            std::size_t length = it->length - (total + it->length - index);
            return size ? it->start[length] : 0;
        }
        else
        {
            total += it->length;
        }
    }
}

const std::list<Span>& Sequence::pieces()
{
    return chain;
}

const char* Sequence::append(const std::string& text)
{
    for (const auto ch : text)
        modify[offset++] = ch;

    return &modify[offset - text.size()];
}

void Sequence::print()
{
    for (const auto& span : chain)
    {
        for (size_t i = 0; i < span.length; ++i)
            std::cout << span.start[i];
    }

    for (const auto& span : chain)
    {
        std::cout << "[" << span.length << "]" << " ";
    }

    std::cout << '\n' << std::endl;
}
