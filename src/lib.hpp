#pragma once
#include <memory>
#include <stdexcept>

extern int not_random();

struct List4 {
    int value;

  private:
    std::unique_ptr<List4> m_next;
    List4 *prev = nullptr;

  public:
    explicit List4(int v) noexcept : value{v} {}
    List4(int v, List4 *arg_prev) noexcept : value{v}, prev{arg_prev} {}
    // ^ То же, что value = v, но использует конструктор, а не присваивание
    List4(List4 const &other, List4 *arg_prev = nullptr)
        : value{other.value},
          m_next{other.m_next ? std::make_unique<List4>(*other.m_next, this)
                              : nullptr},
          prev(arg_prev) {}
    auto operator=(List4 const &other) -> List4 & {
        if (this != &other) {
            value = other.value;
            m_next = other.m_next ? std::make_unique<List4>(*other.m_next, this)
                                  : nullptr;
        }
        return *this;
    }
    List4(List4 &&other) noexcept
        : value{other.value}, m_next{std::move(other.m_next)} {
        if (m_next)
            m_next->prev = this;
    }
    auto operator=(List4 &&other) noexcept -> List4 & {
        value = other.value;
        m_next = std::move(other.m_next);
        if (m_next)
            m_next->prev = this;
        return *this;
    }
    auto &unsafe_next() noexcept { return *m_next; }
    auto const &unsafe_next() const noexcept { return *m_next; }
    auto &next() {
        if (m_next)
            return *m_next;
        throw std::logic_error("calling next on null");
    }
    auto next() const -> List4 &;

    auto has_next() const { return bool{m_next}; }

    void insert_after(List4 &&l);
};

struct List3; // Forward declaration

struct List3Head {
    std::shared_ptr<List3> head;
};