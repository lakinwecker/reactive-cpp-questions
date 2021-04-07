#include "questionnav.h"

#include <vivid/vivid.h>
using namespace vivid::ansi;

nav::Action nav::fromChar(Model questions, char c) {
    if (c == 'j') return Next{};
    if (c == 'k') return Prev{};
    if (c == terminal::RETURN) return EnterQuestion{};
    return Nothing{};
}

nav::Model nav::update(Model questions, Action a) {
    std::visit(lager::visitor{
        [&](Next) { questions = next(questions); },
        [&](Prev) { questions = prev(questions); },
        [&](auto) { },
    }, a);
    return questions;
}

void nav::display(Model questions) {
    for(auto const &q : questions.previousList) {
        std::cout << green << q << std::endl;
    }
    std::cout << white << questions.current << std::endl;
    for(auto const &q : questions.nextList) {
        std::cout << grey100 << q << std::endl;
    }
}
