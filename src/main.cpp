#include "terminal.h"
#include "zippered.h"

#include <cstddef>
#include <iostream>
#include <optional>
#include <variant>
#include <vivid/vivid.h>
#include <immer/flex_vector.hpp>
#include <lager/util.hpp>

using namespace vivid::ansi;

struct YesNoQuestion {
    std::string question;
    std::optional<bool> answer;
};
struct ShortAnswerQuestion {
    std::string question;
    std::string answer;
};
//using Question = std::variant<YesNoQuestion, ShortAnswerQuestion>;
struct Question {
    std::string question;
};

struct Nothing {};
struct Next {};
struct Prev {};
struct Answer {
    std::string answer;
};
using Action = std::variant<Nothing, Next, Prev, Answer>;

using Questionnaire = ZipperedList<Question>;

struct Model {
    Questionnaire questions;
};

Model init() {
    return {
        Questionnaire{
            Question{"Are you old enough to drink?"},
            {},
            {{"Do you live in Canada?"}, {"Where do you live?"}}
        }
    };
}

Model update(Model m, Action a) {
    std::visit(lager::visitor{
        [&](Nothing) { },
        [&](Answer) { },
        [&](Next) { m.questions = next(m.questions); },
        [&](Prev) { m.questions = prev(m.questions); },
    }, a);
    return m;
}

void display(Model m) {
    terminal::clear();
    for(auto const &q : m.questions.previousList) {
        std::cout << green << q.question << std::endl;
    }
    std::cout << white << m.questions.current.question << std::endl;
    for(auto const &q : m.questions.nextList) {
        std::cout << grey100 << q.question << std::endl;
    }
}

Action fromChar(char c) {
    if (c == 'n') return Next{};
    if (c == 'p') return Prev{};
    return Nothing{};
}

int main() {
    Model state = init();
    while (true) {
        display(state);
        Action a = fromChar(terminal::nextCharacter());
        state = update(state, a);
    }
    return EXIT_SUCCESS;
}
