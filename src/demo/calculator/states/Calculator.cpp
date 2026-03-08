/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Calculator.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see Calculator.hpp.
///

#include <algorithm>
#include <cctype>
#include <stack>

#include "Calculator.hpp"
#include "core/logger/Logger.hpp"
#include "imgui/imgui.h"

namespace Graphite::Demo::Calculator::States {

namespace Utils {

template <typename OnClickCallable>
void DrawButton(char const* label, OnClickCallable&& on_click)
{
    if (ImGui::Button(label, ImVec2(60, 60)))
    {
        on_click();
    }
}

// TODO: Move to some common lib in the future
struct CharArray
{
    CharArray(std::size_t raw_size)
        : data{std::make_unique<char[]>(raw_size + 1)}, size{raw_size + 1}
    {
    }

    std::unique_ptr<char[]> data{nullptr};
    std::size_t size{0};
};

CharArray toCharArray(CalculatorState::EquationType const& chars)
{
    CharArray out{chars.size()};

    std::copy(chars.begin(), chars.end(), out.data.get());
    out.data[chars.size()] = '\0';

    return out;
}

bool isOperation(char const chr) noexcept
{
    using namespace std::string_view_literals;
    static auto constexpr c_operations{"<A%/x+-=,"sv};
    return c_operations.contains(chr);
}

inline bool canPushOperation(CalculatorState::EquationType const& equation, char const chr) noexcept
{
    return !equation.empty() && isOperation(chr) && !isOperation(equation.back());
}

namespace EventHandlers {

auto makePushChar(CalculatorState::EquationType& equation, char const chr)
{
    return [&equation, chr]() {
        LOG_INFO("Push '{}' to equation {}", chr, equation);
        equation.push_back(chr);
    };
}

auto makePushOperation(CalculatorState::EquationType& equation, char const chr)
{
    return [&equation, chr]() {
        if (canPushOperation(equation, chr))
        {
            LOG_INFO("Push '{}' to equation {}", chr, equation);
            equation.push_back(chr);
        }
    };
}

auto const do_nothing = []() {};

} // namespace EventHandlers

namespace Calculus {

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == 'x' || op == '/')
        return 2;
    if (op == '%')
        return 2;
    return 0;
}

double applyOp(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case 'x':
        return a * b;
    case '/':
        return b != 0 ? a / b : 0;
    case '%':
        return static_cast<int>(a) % static_cast<int>(b);
    default:
        return 0;
    }
}

bool isOpeningBracket(char c)
{
    return c == '(' || c == '[' || c == '{';
}

bool isClosingBracket(char c)
{
    return c == ')' || c == ']' || c == '}';
}

bool bracketsMatch(char open, char close)
{
    return (open == '(' && close == ')') || (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

void solveEquation(CalculatorState::EquationType& equation)
{
    LOG_SCOPE("");

    if (equation.empty())
        return;

    // Replace comma with dot for decimal
    std::string expr = equation;
    std::replace(expr.begin(), expr.end(), ',', '.');

    std::stack<double> values;
    std::stack<char> ops;

    auto applyTopOp = [&]() {
        if (values.size() < 2 || ops.empty())
            return;
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(a, b, op));
    };

    std::string num_str;
    for (size_t i = 0; i < expr.size(); ++i)
    {
        char c = expr[i];
        if (std::isdigit(c) || c == '.')
        {
            num_str.push_back(c);
        }
        else
        {
            if (!num_str.empty())
            {
                values.push(std::stod(num_str));
                num_str.clear();
            }

            if (isOpeningBracket(c))
            {
                ops.push(c);
            }
            else if (isClosingBracket(c))
            {
                // Evaluate until matching opening bracket
                while (!ops.empty() && !isOpeningBracket(ops.top()))
                {
                    applyTopOp();
                }
                if (!ops.empty() && bracketsMatch(ops.top(), c))
                {
                    ops.pop(); // pop opening bracket
                }
                else
                {
                    // Mismatched brackets, ignore or handle error
                    // For now, just ignore
                }
            }
            else if (c == '+' || c == '-' || c == 'x' || c == '/' || c == '%')
            {
                while (!ops.empty() && !isOpeningBracket(ops.top()) &&
                       precedence(ops.top()) >= precedence(c))
                {
                    applyTopOp();
                }
                ops.push(c);
            }
            else
            {
                // Ignore unknown characters
            }
        }
    }

    if (!num_str.empty())
    {
        values.push(std::stod(num_str));
        num_str.clear();
    }

    while (!ops.empty())
    {
        if (isOpeningBracket(ops.top()))
        {
            ops.pop(); // discard unmatched opening bracket
        }
        else
        {
            applyTopOp();
        }
    }

    double result = values.empty() ? 0.0 : values.top();

    // Convert result to string, using comma as decimal
    std::string result_str = std::to_string(result);
    std::replace(result_str.begin(), result_str.end(), '.', ',');

    equation = result_str;
}

} // namespace Calculus

} // namespace Utils

void DrawCalculator(CalculatorState& state)
{
    using namespace Utils;

    ImGui::Begin(
        "##calculator_window",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    auto const display_equation = toCharArray(state.equation);
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText(
        "##calculator_display",
        display_equation.data.get(),
        display_equation.size,
        ImGuiInputTextFlags_ReadOnly);

    DrawButton("<", [&equation = state.equation]() {
        if (!equation.empty())
        {
            LOG_INFO("Equation pop back");
            equation.pop_back();
        }
        else
        {
            LOG_INFO("Equation cannot pop back ~ reason: empty");
        }
    });
    ImGui::SameLine();
    DrawButton("AC", [&equation = state.equation]() { equation.clear(); });
    ImGui::SameLine();
    DrawButton("%", EventHandlers::makePushOperation(state.equation, '%'));
    ImGui::SameLine();
    DrawButton("/", EventHandlers::makePushOperation(state.equation, '/'));

    DrawButton("7", EventHandlers::makePushChar(state.equation, '7'));
    ImGui::SameLine();
    DrawButton("8", EventHandlers::makePushChar(state.equation, '8'));
    ImGui::SameLine();
    DrawButton("9", EventHandlers::makePushChar(state.equation, '9'));
    ImGui::SameLine();
    DrawButton("x", EventHandlers::makePushOperation(state.equation, 'x'));

    DrawButton("4", EventHandlers::makePushChar(state.equation, '4'));
    ImGui::SameLine();
    DrawButton("5", EventHandlers::makePushChar(state.equation, '5'));
    ImGui::SameLine();
    DrawButton("6", EventHandlers::makePushChar(state.equation, '6'));
    ImGui::SameLine();
    DrawButton("-", EventHandlers::makePushOperation(state.equation, '-'));

    DrawButton("1", EventHandlers::makePushChar(state.equation, '1'));
    ImGui::SameLine();
    DrawButton("2", EventHandlers::makePushChar(state.equation, '2'));
    ImGui::SameLine();
    DrawButton("3", EventHandlers::makePushChar(state.equation, '3'));
    ImGui::SameLine();
    DrawButton("+", EventHandlers::makePushOperation(state.equation, '+'));

    DrawButton("~", EventHandlers::do_nothing);
    ImGui::SameLine();
    DrawButton("0", EventHandlers::makePushChar(state.equation, '0'));
    ImGui::SameLine();
    DrawButton(",", [&equation = state.equation]() {
        if (!equation.empty() && std::isdigit(equation.back()))
        {
            LOG_INFO("Push ',' to equation {}", equation);
            equation.push_back(',');
        }
    });
    ImGui::SameLine();
    DrawButton("=", [&equation = state.equation]() { Calculus::solveEquation(equation); });

    DrawButton("(", [&equation = state.equation]() { equation.push_back('('); });
    ImGui::SameLine();
    DrawButton("[", [&equation = state.equation]() { equation.push_back('['); });
    ImGui::SameLine();
    DrawButton("{", [&equation = state.equation]() { equation.push_back('{'); });

    DrawButton(")", [&equation = state.equation]() { equation.push_back(')'); });
    ImGui::SameLine();
    DrawButton("]", [&equation = state.equation]() { equation.push_back(']'); });
    ImGui::SameLine();
    DrawButton("}", [&equation = state.equation]() { equation.push_back('}'); });

    ImGui::End();
}

} // namespace Graphite::Demo::Calculator::States
