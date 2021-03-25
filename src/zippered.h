#pragma once

#include <immer/flex_vector.hpp>

template <typename T>
struct ZipperedList {
    T current;
    immer::flex_vector<T> previousList;
    immer::flex_vector<T> nextList;
};

template <typename T>
ZipperedList<T> next(ZipperedList<T> q) {
    if (q.nextList.size() == 0) { return q; }

    return ZipperedList<T>{
        q.nextList.front(),
        q.previousList.push_back(q.current),
        q.nextList.drop(1)
    };
}

template <typename T>
ZipperedList<T> prev(ZipperedList<T> q) {
    if (q.previousList.size() == 0) { return q; }

    return ZipperedList<T>{
        q.previousList.back(),
        q.previousList.take(q.previousList.size()-1),
        q.nextList.push_front(q.current)
    };
}
