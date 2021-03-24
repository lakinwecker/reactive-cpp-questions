#include "terminal.h"

#include <cstddef>
#include <iostream>
#include <vivid/vivid.h>
#include <lager/util.hpp>

using namespace vivid::ansi;

struct Action {
    char c;
};

struct Model {
    char c;
};

Model init() {
    return {' '};
}

Model update(Model const &m, Action const &a) {
    return {a.c};
}

void display(Model const &m) {
    terminal::clear();
    std::cout << grey100 << "[" << white << m.c << grey100 << "]" << std::endl;
}

int main() {
    std::cout << "main!" << std::endl;
    Model state;
    while (true) {
        display(state);
        Action a{terminal::nextCharacter()};
        state = update(state, a);
    }
    return EXIT_SUCCESS;
}
