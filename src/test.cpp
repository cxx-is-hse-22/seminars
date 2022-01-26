#include "lib.hpp"
#include <doctest.h>
#include <memory>

TEST_CASE("Something works") { REQUIRE(not_random() == not_random()); }

TEST_CASE("byPosIndex") {
    auto l = std::make_unique<ListElement>(1);
    auto *p = l.get();
    for (int i = 2; i < 7; ++i) {
        p->next = std::make_unique<ListElement>(i);
        p = p->next.get();
    }
    REQUIRE(byPosIndex(l.get(), 0)->value == 1);
    REQUIRE(byPosIndex(l.get(), 1)->value == 2);
    REQUIRE(byPosIndex(l.get(), 2)->value == 3);
    REQUIRE(byPosIndex(l.get(), 3)->value == 4);
    REQUIRE(byPosIndex(l.get(), 4)->value == 5);
    REQUIRE(byPosIndex(l.get(), 5)->value == 6);
    REQUIRE(byPosIndex(l.get(), 6) == nullptr);
}

TEST_CASE("byNegIndex") {
    auto l = std::make_unique<ListElement>(1);
    auto *p = l.get();
    for (int i = 2; i < 7; ++i) {
        p->next = std::make_unique<ListElement>(i);
        p = p->next.get();
    }
    REQUIRE(byNegIndex(l.get(), 0)->value == 6);
    REQUIRE(byNegIndex(l.get(), 1)->value == 5);
    REQUIRE(byNegIndex(l.get(), 2)->value == 4);
    REQUIRE(byNegIndex(l.get(), 3)->value == 3);
    REQUIRE(byNegIndex(l.get(), 4)->value == 2);
    REQUIRE(byNegIndex(l.get(), 5)->value == 1);
    REQUIRE(byNegIndex(l.get(), 6) == nullptr);
}

TEST_CASE("middle") {
    auto l = std::make_unique<ListElement>(1);
    auto *p = l.get();
    for (int i = 2; i < 7; ++i) {
        p->next = std::make_unique<ListElement>(i);
        p = p->next.get();
        auto *middle = listMiddle(l.get());
        REQUIRE(middle);
        REQUIRE(middle->value == (i + 1) / 2);
    }
}

TEST_CASE("cycle") {
    auto l = std::make_shared<ListElementShared>(1);
    auto *p = l.get();
    for (int i = 2; i < 7; ++i) {
        p->next = std::make_shared<ListElementShared>(i);
        p = p->next.get();
    }
    REQUIRE(!hasCycles(l.get()));
    p->next = l->next;
    REQUIRE(hasCycles(l.get()));
    p->next = nullptr;
}