#include "questions.h"

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
std::ostream &operator<<(std::ostream &out, ShortAnswerQuestion q) {
    out << q.question
        << " ["
        << q.answer.value_or(" ")
        << "]";
    return out;
}

std::ostream &operator<<(std::ostream &out, Question q) {
    std::visit(lager::visitor{
        [&](auto q) { out << q; },
    }, q);
    return out;
}
