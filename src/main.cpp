#include "terminal.h"
#include "zippered.h"

#include <cstddef>
#include <iostream>
#include <optional>
#include <variant>
#include <vivid/vivid.h>
#include <immer/flex_vector.hpp>
#include <lager/util.hpp>
#include <functional>

using namespace vivid::ansi;

template <typename ContainedT, typename ReturnT>
std::optional<ReturnT> map(std::optional<ContainedT> const &opt, std::function<ReturnT(ContainedT)> const &mapper) {
    if (!opt.has_value()) return std::nullopt;
    return mapper(opt.value());
}

struct YesNoQuestion {
    std::string question;
    std::optional<bool> answer;
};
std::ostream &operator<<(std::ostream &out, YesNoQuestion q) {
    out << q.question
        << " ["
        << map<bool, std::string>(
                q.answer,
                [](bool answer) { return answer ? "Y" : "N"; }
            ).value_or(" ")
        << "]";

    return out;
}

struct ShortAnswerQuestion {
    std::string question;
    std::optional<std::string> answer;
};
std::ostream &operator<<(std::ostream &out, ShortAnswerQuestion q) {
    out << q.question
        << " ["
        << q.answer.value_or(" ")
        << "]";
    return out;
}

using Question = std::variant<YesNoQuestion, ShortAnswerQuestion>;
std::ostream &operator<<(std::ostream &out, Question q) {
    std::visit(lager::visitor{
        [&](auto q) { out << q; },
    }, q);
    return out;
}

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
            YesNoQuestion{"Are you old enough to drink?", true},
            {},
            {
                YesNoQuestion{"Do you live in Canada?"},
                ShortAnswerQuestion{"Where do you live?", "Calgary"},
                ShortAnswerQuestion{"What is your name?"}
            }
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
    //terminal::clear();
    for(auto const &q : m.questions.previousList) {
        std::cout << green << q << std::endl;
    }
    std::cout << white << m.questions.current << std::endl;
    for(auto const &q : m.questions.nextList) {
        std::cout << grey100 << q << std::endl;
    }
}

Action fromChar(char c) {
    if (c == 'j') return Next{};
    if (c == 'k') return Prev{};
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
