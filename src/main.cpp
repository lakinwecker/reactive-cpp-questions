#include "terminal.h"

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

struct Questionnaire {
    Question curQuestion;
    immer::flex_vector<Question> prevQuestions;
    immer::flex_vector<Question> nextQuestions;
};

struct Model {
    Questionnaire questions;
};

Questionnaire next(Questionnaire q) {
    return Questionnaire{
        q.nextQuestions.front(),
        q.prevQuestions.push_back(q.curQuestion),
        q.nextQuestions.drop(1)
    };
}
Questionnaire prev(Questionnaire q) {
    return Questionnaire{
        q.prevQuestions.back(),
        q.prevQuestions.take(q.prevQuestions.size()-1),
        q.nextQuestions.push_front(q.curQuestion)
    };
}

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
    for(auto const &q : m.questions.prevQuestions) {
        std::cout << green << q.question << std::endl;
    }
    std::cout << white << m.questions.curQuestion.question << std::endl;
    for(auto const &q : m.questions.nextQuestions) {
        std::cout << grey100 << q.question << std::endl;
    }

    //std::cout << grey100 << "[" << white << m.c << grey100 << "]" << std::endl;
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
