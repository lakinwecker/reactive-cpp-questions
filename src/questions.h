#pragma once
#include "utils.h"
#include "zippered.h"

#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <lager/util.hpp>

struct YesNoQuestion {
    std::string question;
    std::optional<bool> answer;
};
std::ostream &operator<<(std::ostream &out, YesNoQuestion q);

struct ShortAnswerQuestion {
    std::string question;
    std::optional<std::string> answer;
};
std::ostream &operator<<(std::ostream &out, ShortAnswerQuestion q);

using Question = std::variant<YesNoQuestion, ShortAnswerQuestion>;
std::ostream &operator<<(std::ostream &out, Question q);

using Questionnaire = ZipperedList<Question>;
