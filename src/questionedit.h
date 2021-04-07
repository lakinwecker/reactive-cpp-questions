#pragma once
#include "terminal.h"
#include "questions.h"

#include <functional>
#include <variant>

namespace edit {
    struct Nothing {};
    struct Done {};
    using Action = std::variant<Nothing, Done>;
    using Model = Questionnaire;

    Model update(Model questions, Action a);

    Action fromChar(Model questions, char c);

    void display(Model questions);
}
