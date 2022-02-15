// Для сборки нужно установить conan: https://conan.io/downloads.html
#include "lib.hpp"
#include <array>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vector>

/*
@dataclass
class List1:
    value: int
    next: 'List1' = None
    def __init__(self, v):
        self.value = v
*/
struct List1 {
    int value;
    std::shared_ptr<List1> next /* = nullptr */;
    List1(int v) { value = v; }
};
auto print(List1 const &list) -> void {
    fmt::print("{}{}", list.value, (list.next ? ", " : "\n"));
    // cond ? expr1 : expr2 это как в python было expr1 if cond else expr2
    if (list.next)
        print(*list.next);
}
auto step1() -> void {
    print(3);
    print(List1{3}); // то же самое
}

struct List2 {
    int value;
    std::shared_ptr<List2> next /* = nullptr */;
    explicit List2(int v) { value = v; }
};
auto print2(List2 const &list) -> void {
    fmt::print("{}{}", list.value, (list.next ? ", " : "\n"));
    // cond ? expr1 : expr2 это как в python было expr1 if cond else expr2
    if (list.next)
        print2(*list.next);
}
auto step2() {
    // print2(3); // ERROR! потому что explicit
    print2(List2{3}); // то же самое
}

struct Stack {
    static const int max_number_of_elements = 32;
    // Stack::max_number_of_elements == 32
    std::array<int, max_number_of_elements> elements;
    size_t top = 0;

    auto push_back(int value) {
        if (top >= max_number_of_elements)
            throw std::out_of_range("stack is full"); // raise в python
        elements[top] = value;
        ++top;
        // elements[top++] = value;
    }

    auto pop_back() {
        if (top == 0)
            throw std::out_of_range("stack is empty");
        --top;
        return elements[top];
        // return elements[--top];
    }

    Stack() = default;
    // ^ пусть компилятор сам этот конструктор напишет, он умеет
    Stack(std::initializer_list<int> list) {
        for (auto elem : list)
            push_back(elem);
    }
    int operator[](size_t index) const {
        if (index < top)
            return elements[top - index - 1];
        throw std::out_of_range("index too large");
    }
};
// std::ostream &operator<<(std::ostream &stream, Stack const &stack) {
auto operator<<(std::ostream &stream, Stack const &stack) -> std::ostream & {
    stream << "Stack{";
    for (size_t i = 0; i < stack.top; ++i)
        stream << stack[i] << (i == stack.top - 1 ? "" : ", ");
    stream << "}";
    return stream;
}
auto step3() {
    auto s1 = Stack();
    auto s2 = Stack{1, 2, 3};
    // Если компилятор видит '{', он ищет конструктор. принимающий
    // std::initializer_list. Если он есть, он и вызывается. Если его нет,
    // элементы в {} рассматриваются как отдельные аргументы конструктора
    std::cout << s2 << "\n";
    fmt::print("{}\n", s1);
    try {
        s1.pop_back();
    } catch (std::out_of_range const &error) {
        fmt::print(std::cerr, "got error: {}\n", error.what());
        // throw; // как raise
    }
    fmt::print("s2[1]: {}\n", s2[1]);
}

struct example1 {
    int v;
    explicit example1(int value) { v = value; }
    explicit example1(std::initializer_list<int> value) {
        v = static_cast<int>(value.size());
    }
    explicit example1(std::initializer_list<int> value,
                      std::initializer_list<int> /*unused*/) {
        v = static_cast<int>(value.size());
    }
};

auto step4() {
    auto e1 = example1(2);
    auto e2 = example1{2};
    // Компилятор пытается превратить example1{2} в
    // example1(std::initializer_list{2})
    auto e3 = example1({2}, {3});
    [[maybe_unused]] auto i1 = int(3.14);
    // [[maybe_unused]] auto i2 = int{3.14};
    // [[maybe_unused]] auto i3 = char{200};

    fmt::print("e1 = {}, e2 = {}, e3 = {}\n", e1.v, e2.v, e3.v);
}
struct List3 {
    int value = 0;
    std::shared_ptr<List3> next /* = nullptr */;
    explicit List3(int v) { value = v; }
    List3(List3 const &other) {
        value = other.value;
        // if (other.next)
        //     next = std::make_shared<List3>(*other.next);
            // List3{*other.next}
        auto *from = other.next.get();
        auto *to = this;
        while (from != nullptr) {
            to->next = std::make_shared<List3>(from->value);
            to = to->next.get();
            from = from->next.get();
        }
        // *next = *other.next;
    }
    auto operator=(List3 const &other) -> List3 & {
        if (this == &other) // если l3 = l3; просто оптимизация
            return *this;
        value = other.value;
        if (other.next)
            next = std::make_shared<List3>(*other.next);
        else
            next = nullptr;
        return *this;
        // Зачем возвращать *this?
        // l1 = l2 = l3
        // if ((l1 = l2).value != 0)
    }
    List3(List3 &&other) = default;
    auto operator=(List3 &&other) noexcept -> List3 & {
        value = std::move(other.value);
        next = std::move(other.next);
        return *this;
    }

    ~List3() { fmt::print("List3 destroyed\n"); }
    // Rule of 0: не пишите ни оператор/конструктор копирования,
    // ни оператор/конструктор перемещения, ни деструктор
    // Rule of 5: если реализуете одного из них, реализуйте все 5
};

List3 make_mylist() {
    List3 l(1);
    l.next = std::make_shared<List3>(2);
    l.next->next = std::make_shared<List3>(3);
    return l;
}

auto step5() {
    auto l1 = List3(3);
    l1.next = std::make_shared<List3>(2);
    auto l2 = l1; // auto l2{l1}; auto l2 = List3{l1};
    l2.next->value = 4;
    fmt::print("l1: {}, {}\n", l1.value, l1.next->value);
    l1 = l2;
    fmt::print("l1: {}, {}\n", l1.value, l1.next->value);
}

auto f() /*noexcept*/ { throw std::invalid_argument(""); }

auto step6() {
    {
        fmt::print("step6() start\n");
        auto l1 = List3(0);
        l1 = make_mylist();
        auto l2 = std::move(l1);
        fmt::print("step6() end\n");
    }
    try {
        f();
    } catch (std::invalid_argument const &error) {
        fmt::print(std::cerr, "error: {}\n", error.what());
    }
}

auto print(List4 const &l) -> void {
    std::cout << l.value << ", ";
    if (l.has_next())
        print(l.unsafe_next());
}

auto step7() {
    auto l1 = List4(3);
    l1.insert_after(List4(2));
    auto l2 = l1;
    std::swap(l2, l1);
    auto l3 = std::move(l1);
}

int main() {
    // syntax:
    // https://docs-python.ru/tutorial/vstroennye-funktsii-interpretatora-python/funktsija-format/
    std::cout << fmt::format("not_random number is {0:04}\n", not_random());
    fmt::print("Another string and a vector {}\n", std::vector{1, 2, 3, 4});
    step1();
    step2();
    step3();
    step4();
    step5();
    step6();
    step7();
    return 0;
}