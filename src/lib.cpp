#include "lib.hpp"

int not_random() { return 3; }

void List4::insert_after(List4 &&l) {
    auto *last = &l;
    while (last->m_next)
        last = last->m_next.get();
    last->m_next = std::move(m_next);
    if (last->m_next)
        last->m_next->prev = last;
    m_next = std::make_unique<List4>(std::move(l));
    m_next->prev = this;
}

auto List4::next() const -> List4 & {
    if (m_next)
        return *m_next;
    throw std::logic_error("calling next on null");
}