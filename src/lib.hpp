#pragma once
#include <iterator>
#include <string_view>

extern int not_random();
struct words_iterator {
    std::string_view str; // Остаток строки
    size_t length; // Длина текущего слова
    words_iterator(std::string_view str);
    // Определяет длину первого слова
    auto normalize() -> void;

    auto operator==(words_iterator const &other) const {
        return str.data() == other.str.data() && str.size() == other.str.size();
    }
    auto operator!=(words_iterator const &other) const {
        return !(*this == other);
    }

    static auto isalpha(char c) -> bool;
    auto operator*() const -> std::string_view;
    auto operator++() -> words_iterator &;
    auto operator++(int) -> words_iterator {
        auto old = *this;
        ++*this;
        return old;
    }
    auto operator->() const { return **this; }
};

struct words_range {
    std::string_view str;
    auto begin() const { return words_iterator{str}; }
    auto end() const { return words_iterator{str.substr(str.size())}; }
};

template <typename Iterator>
struct enumerate_iterator
    : std::iterator<std::forward_iterator_tag,
                    std::pair<size_t, typename Iterator::value_type>> {
    Iterator iterator;
    size_t index = 0;
    enumerate_iterator(Iterator it, size_t idx = 0) : iterator(it), index(idx) {}
    bool operator==(enumerate_iterator const& other) const {
        return iterator == other.iterator;
    }
    bool operator!=(enumerate_iterator const& other) const {
        return iterator != other.iterator;
    }
    auto operator++() -> enumerate_iterator & {
        ++iterator;
        ++index;
        return *this;
    }
    auto operator++(int) {
        return enumerate_iterator{iterator++, index++};
    }
    auto operator*() const {
        return std::pair{index, *iterator};
    }
};

// Class Template Argument Deduction
template <typename Iterator>
enumerate_iterator(Iterator) -> enumerate_iterator<Iterator>;
template <typename Iterator>
enumerate_iterator(Iterator, size_t) -> enumerate_iterator<Iterator>;

template<typename Range>
struct enumerate {
    Range &range;
    enumerate(Range &rng) : range(rng) {}
    auto begin() const { return enumerate_iterator{range.begin()}; }
    auto end() const { return enumerate_iterator{range.end(), range.size()}; }
};

template <typename Range> enumerate(Range &range) -> enumerate<Range>;

// TODO: сделать enumerate_iterator итератором произвольного доступа