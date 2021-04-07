#include "questionedit.h"

#include <vivid/vivid.h>
using namespace vivid::ansi;

edit::Action edit::fromChar(Model questions, char c) {
    if (c == terminal::ESCAPE) return Done{};
    return Nothing{};
}

edit::Model edit::update(Model questions, Action a) {
    return questions;
}

void edit::display(Model questions) {
    for(auto const &q : questions.previousList) {
        std::cout << green << q << std::endl;
    }
    std::cout << red << "EDIT: ";
    std::cout << white << questions.current << std::endl;
    for(auto const &q : questions.nextList) {
        std::cout << grey100 << q << std::endl;
    }
}
