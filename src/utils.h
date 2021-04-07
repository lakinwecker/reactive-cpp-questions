#pragma once
#include <optional>
#include <functional>

template <typename ContainedT, typename ReturnT>
std::optional<ReturnT> map(
    std::optional<ContainedT> const &opt,
    std::function<ReturnT(ContainedT)> const &mapper
) {
    if (!opt.has_value()) return std::nullopt;
    return mapper(opt.value());
}
