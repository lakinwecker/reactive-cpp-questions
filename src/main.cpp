#include "terminal.h"
#include "utils.h"
#include "questionnav.h"
#include "questionedit.h"
#include "questions.h"

#include <cstddef>
#include <iostream>
#include <optional>
#include <variant>
#include <vivid/vivid.h>
#include <immer/flex_vector.hpp>
#include <lager/util.hpp>
#include <functional>

using namespace vivid::ansi;


using Action = std::variant<nav::Action, edit::Action>;

struct Navigation {
    Questionnaire questions;
};
struct EditQuestion {
    Questionnaire questions;
};
using Model = std::variant<Navigation, EditQuestion>;

Model init() {
    return {
        Navigation{
            Questionnaire{
                YesNoQuestion{"Are you old enough to drink?", true},
                {},
                {
                    YesNoQuestion{"Do you live in Canada?"},
                    ShortAnswerQuestion{"Where do you live?", "Calgary"},
                    ShortAnswerQuestion{"What is your name?"}
                }
            }
        }
    };
}

Model update(Model m, Action a) {
    return std::visit(lager::visitor{
        [&](Navigation nav, nav::Action a) {
            return std::visit(lager::visitor{
                [&](nav::EnterQuestion a) {
                    return Model{EditQuestion{nav.questions}};
                },
                [&](auto a) {
                    nav.questions = nav::update(nav.questions, a);
                    return Model{nav};
                }
            }, a);
        },
        [&](EditQuestion edit, edit::Action a) {
            return std::visit(lager::visitor{
                [&](edit::Done) {
                    return Model{Navigation{edit.questions}};
                },
                [&](auto a) {
                    edit.questions = edit::update(edit.questions, a);
                    return Model{edit};
                }
            }, a);
        },
        [&](auto m, auto a) { return Model{m}; }
    }, m, a);
}

void display(Model m) {
    terminal::clear();
    std::visit(lager::visitor{
        [&](Navigation nav) {
            nav::display(nav.questions);
        },
        [&](EditQuestion edit) {
            edit::display(edit.questions);
        },
    }, m);
}

Action fromChar(Model m, char c) {
    return std::visit(lager::visitor{
        [&](Navigation nav) {
            return Action{nav::fromChar(nav.questions, c)};
        },
        [&](EditQuestion edit) {
            return Action{edit::fromChar(edit.questions, c)};
        }
    }, m);
}

int main() {
    Model state = init();
    while (true) {
        display(state);
        Action a = fromChar(state, terminal::nextCharacter());
        state = update(state, a);
    }
    return EXIT_SUCCESS;
}
