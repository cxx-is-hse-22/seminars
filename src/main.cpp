#include "lib.hpp"
#include <cassert>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

auto sqr(auto a) { return a * a; }

auto bad_increment(int a) { return ++a; } // Не увеличивает переменную
auto increment(int &a) { return ++a; }

auto make_array(int my_size) {
    if (my_size < 0)
        my_size = 0;
    auto array = std::vector(static_cast<std::size_t>(my_size), 0);
    // ^ Создаёт массив длины my_size из нулей
    for (auto i = 0; i < my_size; ++i)
        array[static_cast<std::size_t>(i)] = i;
    return array;
}

auto sum(std::vector<int> const &array) {
    auto result = 0;
    for (auto elem : array)
        result += elem;
    // array[0] = 3; // Error!
    return result;
}

struct Person { // как @dataclass
    std::string name;
    int age;
    bool student = true;

    Person(std::string arg_name, int arg_age) { // как __init__ в python
        name = arg_name;
        age = arg_age;
    }

    auto get_name() const { return name; }

    auto operator==(Person const &other) const -> bool = default;
};

auto get_person_name(Person const &person) { return person.get_name(); }

auto make_person(std::string name, int age) { return Person{name, age}; }

void print(Person const &person) {
    fmt::print("Person(\"{}\", {})\n", person.name, person.age);
    std::cout << fmt::format("Person(\"{}\", {})\n", person.name, person.age);
    std::cout << "Person(\"" << person.name << "\", " << person.age << ")\n";
}

auto main() -> int { // or int main()
    using namespace std::literals;
    std::cout << not_random() << "\n";
    std::cout << sqr(not_random()) << "\n";
    // sqr("123"s); // Error!
    auto value = 0;
    std::cout << value << " ";
    std::cout << increment(value) << " ";
    // increment(3); // error
    std::cout << value << "\n";

    auto const my_size = 1024;
    // нельзя my_size = 1;
    auto array = make_array(my_size);
    std::cout << "Sum of variable array " << sum(array) << "\n";
    std::cout << "Sum of temporary array " << sum(make_array(my_size)) << "\n";
    fmt::print("Small array {}\n", make_array(8));

    auto john = Person{"John", 23};
    std::cout << "Person(\"" << john.name << "\", " << john.age << ")\n";
    john.age = 32;

    auto ptr = std::make_shared<Person>("Joe", 33);
    // std::shared_ptr<Person>(new Person{"Joe", 33})
    // new использовать не надо
    auto ptr2 = ptr;
    auto &person_ref = *ptr; // *ptr превращает (умный) указатель ptr в ссылку
    (*ptr2).student = false;
    ptr2->student = false; // Копия прошлой строки
    std::cout << "Joe student=" << ptr->student << "\n";
    print(*ptr);
    ptr = std::make_shared<Person>("Jack", 55);
    // На Joe остался 1 указатель
    print(person_ref);
    ptr2 = nullptr; // примерно то же, что None в python
    // Joe немедленно удаляется
    // ссылка person_ref содержит некорректное значение
    // читать и писать её нельзя
    // Мы говорим, что shared_ptr "владеет" объектом, на который ссылается
    // А ссылка не "владеет" им
    // Пока жив "владелец", живы все его "владения"
    auto vec = std::vector{1, 2, 3};
    auto &vec0 = vec[0]; // NOLINT
    vec = std::vector<int>{};
    // vec0 теперь неправильный

    auto uptr = std::make_unique<Person>("Joe", 33);
    auto &uperson_ref = *uptr;
    uperson_ref.student = false;
    uptr = nullptr;

    struct ListElement {
        int value;
        // auto std::make_unique<Type>(...) -> std::unique_ptr<Type>
        std::unique_ptr<ListElement> next;
        ListElement(int v) { value = v; }
        auto print() const -> void {
            if (next) {
                std::cout << value << ", ";
                next->print();
            } else
                std::cout << value << "\n";
        }
    };

    auto list_ptr = std::make_unique<ListElement>(1);
    list_ptr->next = std::make_unique<ListElement>(2);
    list_ptr->print();
    // дописали в конец, теперь список длины 2
    {
        auto new_list_elem = std::make_unique<ListElement>(3);
        new_list_elem->print();
        // list_ptr --- это голова (начало) списка
        // new_list_elem->next и list_ptr не могут одновременно владеть объектом
        // 1
        new_list_elem->next = std::move(list_ptr);
        // Теперь list_ptr = nullptr (но это не точно)
        // А new_list_elem->next теперь равен прошлому значению list_ptr
        new_list_elem->print();
        std::swap(list_ptr, new_list_elem);
        // ^ Меняет местами list_ptr и new_list_elem
        // list_ptr, new_list_elem = new_list_elem, list_ptr
        list_ptr->print();
    } // здесь удаляется указатель new_list_elem (==nullptr)
    // дописали в начало, теперь список длины 3

    // list_ptr->next = std::move(list_ptr);
    // ^ Утечка памяти: list_ptr никогда не будет удалён

    struct List2Element {
        int value;
        std::shared_ptr<List2Element> next; // или std::unique_ptr
        // std::shared_ptr<List2> prev; // Неминуемая утечка памяти
        // list2_ref будет владеть собой через list2_ref->next->prev
        std::weak_ptr<List2Element> prev;
        // ^ prev не владеет данными, на которые указывает
        // ^ Для unique_ptr это будет List2* prev;
        List2Element(int v) { value = v; }
    };
    auto list2_ref = std::make_shared<List2Element>(1);
    list2_ref->next = std::make_shared<List2Element>(2);
    list2_ref->next->prev = list2_ref;
    auto second = list2_ref->next;
    {
        auto second_prev = second->prev.lock();
        if (second_prev)
            std::cout << "prev to second is " << second_prev->value << "\n";
    }

    struct List2uElement {
        int value;
        std::unique_ptr<List2uElement> next;
        List2uElement *prev = nullptr;
        List2uElement(int v) { value = v; }
    };

    struct List2u {
        std::unique_ptr<List2uElement> head;
        List2uElement *tail = nullptr;

        auto append(int value) {
            if (head) {
                assert(tail && !tail->next);
                tail->next = std::make_unique<List2uElement>(value);
                tail->next->prev = tail;
                tail = tail->next.get();
            } else {
                head = std::make_unique<List2uElement>(value);
                tail = head.get();
            }
        }
    };

    struct Tree {
        std::shared_ptr<Tree> left, right; // лучше std::unique_ptr
    };
    return 0;
}