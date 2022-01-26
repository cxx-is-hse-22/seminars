#include "lib.hpp"
#include <iostream>

auto not_random() -> int { return 3; } // Definition

auto ListElement::print() const -> void {
    if (next) {
        std::cout << value << ", ";
        next->print();
    } else
        std::cout << value << "\n";
}

auto listSize(ListElement const *list) {
    auto i = 0;
    while (list != nullptr) {
        list = list->next.get();
        ++i;
    }
    return i;
}

auto byNegIndex2(ListElement *list, int n) -> ListElement * {
    auto size = listSize(list);
    if (n >= size)
        return nullptr;
    return byPosIndex(list, size - n - 1);
}

auto byNegIndex(ListElement *list, int n) -> ListElement * {
    auto *ptr = byPosIndex(list, n);
    if (ptr == nullptr)
        return nullptr;
    // Distance from list to ptr is always n elements
    while (ptr->next != nullptr) {
        list = list->next.get();
        ptr = ptr->next.get();
    }
    return list;
}

auto byPosIndex(ListElement *list, int n) -> ListElement * {
    for (int i = 0; i < n; ++i)
        if (list != nullptr)
            list = list->next.get();
        else
            return nullptr;
    return list;
}

auto listMiddle(ListElement *list) -> ListElement * {
    auto *ptr = list->next.get();
    if (ptr != nullptr)
        ptr = ptr->next.get();
    while (ptr != nullptr) {
        list = list->next.get();
        ptr = ptr->next.get();
        if (ptr != nullptr)
            ptr = ptr->next.get();
    }
    return list;
}

auto hasCycles(ListElementShared *list) -> bool {
    auto *ptr = list->next.get();
    while (ptr != nullptr) {
        if (list == ptr)
            return true;
        list = list->next.get();
        ptr = ptr->next.get();
        if (ptr != nullptr)
            ptr = ptr->next.get();
    }
    return false;
}

auto List2u::byPosIndex(int n) const -> List2uElement const * {
    auto *current = head.get();
    for (int i = 0; i < n; ++i)
        if (current != nullptr)
            current = current->next.get();
        else
            return nullptr;
    return current;
}