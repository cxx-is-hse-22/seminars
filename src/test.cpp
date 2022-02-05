#include "lib.hpp"
#include <doctest.h>
#include <memory>

TEST_CASE("Something works") { REQUIRE(not_random() == not_random()); }

TEST_CASE("byPosIndex") {
    auto head = std::make_unique<ListElement>(1);
    auto *tail = head.get();
    for (int i = 2; i < 7; ++i) {
        tail->next = std::make_unique<ListElement>(i);
        tail = tail->next.get();
    }
    REQUIRE(byPosIndex(head.get(), 0)->value == 1);
    REQUIRE(byPosIndex(head.get(), 1)->value == 2);
    REQUIRE(byPosIndex(head.get(), 2)->value == 3);
    REQUIRE(byPosIndex(head.get(), 3)->value == 4);
    REQUIRE(byPosIndex(head.get(), 4)->value == 5);
    REQUIRE(byPosIndex(head.get(), 5)->value == 6);
    REQUIRE(byPosIndex(head.get(), 6) == nullptr);
}

TEST_CASE("byNegIndex") {
    auto head = std::make_unique<ListElement>(1);
    auto *tail = head.get();
    for (int i = 2; i < 7; ++i) {
        tail->next = std::make_unique<ListElement>(i);
        tail = tail->next.get();
    }
    REQUIRE(byNegIndex(head.get(), 0)->value == 6);
    REQUIRE(byNegIndex(head.get(), 1)->value == 5);
    REQUIRE(byNegIndex(head.get(), 2)->value == 4);
    REQUIRE(byNegIndex(head.get(), 3)->value == 3);
    REQUIRE(byNegIndex(head.get(), 4)->value == 2);
    REQUIRE(byNegIndex(head.get(), 5)->value == 1);
    REQUIRE(byNegIndex(head.get(), 6) == nullptr);
}

TEST_CASE("middle") {
    auto head = std::make_unique<ListElement>(1);
    auto *tail = head.get();
    for (int i = 2; i < 7; ++i) {
        tail->next = std::make_unique<ListElement>(i);
        tail = tail->next.get();
        auto *middle = listMiddle(head.get());
        REQUIRE(middle);
        REQUIRE(middle->value == (i + 1) / 2);
    }
}

TEST_CASE("cycle") {
    auto head = std::make_shared<ListElementShared>(1);
    auto *tail = head.get();
    for (int i = 2; i < 7; ++i) {
        tail->next = std::make_shared<ListElementShared>(i);
        tail = tail->next.get();
    }
    REQUIRE(!hasCycles(head.get()));
    tail->next = head->next;
    REQUIRE(hasCycles(head.get()));
    auto size = getCycleLength(findCycles(head.get()));
    REQUIRE(size == 5);
    REQUIRE(findLastCycleNode(head.get(), size) == tail);
    tail->next = head;
    size = getCycleLength(findCycles(head.get()));
    REQUIRE(size == 6);
    REQUIRE(findLastCycleNode(head.get(), size) == tail);
    tail->next = nullptr;
}