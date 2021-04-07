#pragma once
#include "terminal.h"
#include "questions.h"

#include <functional>
#include <variant>

namespace nav {
    struct Nothing {};
    struct Next {};
    struct Prev {};
    struct EnterQuestion {};
    using Action = std::variant<Nothing, Next, Prev, EnterQuestion>;
    using Model = Questionnaire;

    Model update(Model questions, Action a);

    Action fromChar(Model questions, char c);

    void display(Model questions);
}
