#pragma once
#include <cassert>
#include <memory>

extern auto not_random() -> int; // Declaration

struct ListElement {
    int value;
    // auto std::make_unique<Type>(...) -> std::unique_ptr<Type>
    std::unique_ptr<ListElement> next;
    ListElement(int v) { value = v; }
    auto print() const -> void;
};

extern auto byPosIndex(ListElement *list, int n) -> ListElement *;
extern auto byNegIndex(ListElement *list, int n) -> ListElement *;
auto listMiddle(ListElement *list) -> ListElement *;

struct List2uElement {
    int value;
    std::unique_ptr<List2uElement> next;
    List2uElement *prev = nullptr;
    List2uElement(int v) { value = v; }
};

struct List2u {
    std::unique_ptr<List2uElement> head;
    List2uElement *tail = nullptr;

    auto append(int value) -> List2uElement & {
        if (head) {
            assert(tail && !tail->next); // NOLINT
            tail->next = std::make_unique<List2uElement>(value);
            tail->next->prev = tail;
            tail = tail->next.get();
        } else {
            head = std::make_unique<List2uElement>(value);
            tail = head.get();
        }
        return *tail;
    }

    auto erase(List2uElement &elem) {
        auto *next = elem.next.get();
        auto *prev = elem.prev;
        if (prev != nullptr)
            prev->next = std::move(elem.next);
        else
            head = std::move(elem.next);
        // elem после этого удаляется,
        // так как prev->next им больше не владеет
        if (next != nullptr)
            next->prev = next;
        else
            tail = next;
    }

    auto byPosIndex(int n) const -> List2uElement const *;
};

struct ListElementShared {
    int value;
    std::shared_ptr<ListElementShared> next;
    ListElementShared(int v) { value = v; }
};

auto hasCycles(ListElementShared *list) -> bool;
auto findCycles(ListElementShared *list) -> ListElementShared *;
auto getCycleLength(ListElementShared *list) -> int;
auto findLastCycleNode(ListElementShared *list, int size)
    -> ListElementShared *;