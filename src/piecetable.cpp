#include "piecetable.h"

Sequence::Sequence(const std::string& file_name)
    : offset(0), active(nullptr)
{
    int fd = open(file_name.c_str(), O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    size = sb.st_size;
    original = static_cast<const char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));

    head = new Span{nullptr, 0, 0};
    tail = new Span{nullptr, 0, 0};

    tail->prev = head;
    head->next = tail;

    insert(tail, new Span{original, static_cast<size_t>(sb.st_size), true});

//    head->next->next = tail;
//    tail->prev = head->next;
//    tail->next = nullptr;
}

Sequence::~Sequence()
{
    // Nothing to do
}

SpanRange Sequence::insert_char(std::size_t index, char ch)
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
        active = insert(++span, new Span{start, 1, false});
        return {};
    }

    std::size_t length = span->length - (total + span->length - index);

    Span* one   = new Span{span->start, length, span->original};
    Span* two   = new Span{start, 1, false};
    Span* three = new Span{span->start + length, span->length - length, span->original};

    auto it = erase(span);
    Span* end = it;

    // Insert new spans
    it = insert(it, three);
    it = insert(it, two);

    active = it;

    // Avoid inserting empty span
    if (length)
        insert(it, one);
    else
        delete one;

    return {it, three};
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
        insert(++span, new Span{start, text.length(), false});
        return;
    }

    std::size_t length = span->length - (total + span->length - index);

    Span* one   = new Span{span->start, length, span->original};
    Span* two   = new Span{start, text.size(), false};
    Span* three = new Span{span->start + length, span->length - length, span->original};

    auto it = erase(span);

    // Insert new spans
    it = insert(it, three);
    it = insert(it, two);
         insert(it, one);
}

const char* Sequence::append_text(const std::string& text)
{
    for (const auto ch : text)
        modify[offset++] = ch;

    return &modify[offset - text.size()];
}

void Sequence::remove_char(std::size_t index)
{
    std::size_t total;
    auto active = get_span(index, total);
    if (total + active->length - 1 == index)
    {
        if (--active->length || active == head)
            return;
        else
            erase(active);
    }
    else
    {
        std::size_t length = active->length - (total + active->length - index);
        Span* one = new Span{active->start, length, active->original};
        Span* two = new Span{active->start + length + 1, active->length - length - 1, active->original };
        auto it = erase(active);
        it = insert(it, two);

        if (one->length)
            insert(it, one);
        else
            delete one;
    }
}

char Sequence::get_ch(std::size_t index)
{
    if (size)
    {
        std::size_t total = 0;
        Span* it = get_span(index, total);
        std::size_t length = it->length - (total + it->length - index);
        return it == head ? it->next->start[length] : it->start[length];
    }

    return 0;
}

Span* Sequence::get_span(std::size_t index, std::size_t& total)
{
    total = 0;    
    for (Span* it = head; it->next; it = it->next)
        if (index >= total && index < total + it->length)
            return it;
        else
            total += it->length;

    return head;
}

Span* Sequence::insert(Span* pos, Span* node)
{
    pos->prev->next = node;
    node->prev = pos->prev;
    node->next = pos;
    pos->prev = node;
    return node;
}

Span* Sequence::erase(Span* node)
{
    Span* ret = node->next;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    return ret;
}

Span* Sequence::pieces()
{
    return head;
}

const char* Sequence::start()
{
    return head->next->start;
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

    for (Span* it = head; it->next; it = it->next)
    {
        std::cout << "[" << it->length << "]" << " ";
    }

    std::cout << '\n' << std::endl;
}
