#pragma once

// source https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts>
struct Visitor : Ts... { using Ts::operator()...; };

template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;