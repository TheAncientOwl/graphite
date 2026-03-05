/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Logger.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see Logger.hpp.
///

#include "Logger.hpp"

#include <ctime>
#include <iomanip>

using namespace std::string_literals;

namespace Graphite::Core::Logger {

inline std::string_view getLogColor(LogLevel level) noexcept
{
    static constexpr auto reset = "\033[0m";
    static constexpr auto trace = "\033[97m";
    static constexpr auto info = "\033[34m";
    static constexpr auto warn = "\033[33m";
    static constexpr auto error = "\033[31m";
    static constexpr auto critical = "\033[91m";
    static constexpr auto debug = "\033[92m";
    static constexpr auto scope = "\033[95m";

    switch (level)
    {
    case LogLevel::Trace:
        return trace;
    case LogLevel::Info:
        return info;
    case LogLevel::Warn:
        return warn;
    case LogLevel::Error:
        return error;
    case LogLevel::Critical:
        return critical;
    case LogLevel::Debug:
        return debug;
    case LogLevel::Scope:
        return scope;
    default:
        return reset;
    }
}

inline std::string_view getLogLevelName(LogLevel level) noexcept
{
    static constexpr auto trace = "Trace";
    static constexpr auto info = "Info";
    static constexpr auto warn = "Warn";
    static constexpr auto error = "Error";
    static constexpr auto critical = "Critical";
    static constexpr auto debug = "Debug";
    static constexpr auto scope = "Scope";
    static constexpr auto unknown = "Unknown";

    switch (level)
    {
    case LogLevel::Trace:
        return trace;
    case LogLevel::Info:
        return info;
    case LogLevel::Warn:
        return warn;
    case LogLevel::Error:
        return error;
    case LogLevel::Critical:
        return critical;
    case LogLevel::Debug:
        return debug;
    case LogLevel::Scope:
        return scope;
    default:
        return unknown;
    }
}

inline std::string_view getSeparatorColor() noexcept
{
    static constexpr auto gray = "\033[90m";
    return gray;
}

Logger& Logger::instance()
{
    static Logger logger{};
    return logger;
}

void Logger::enqueue(LogMessage&& msg)
{
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_queue.emplace(std::move(msg));
    }
    m_cv.notify_one();
}

Logger::~Logger()
{
    m_running = false;
    m_cv.notify_one();
    if (m_worker.joinable())
        m_worker.join();
}

Logger::Logger() : m_running{true}, m_worker{&Logger::processQueue, this}
{
}

void Logger::processQueue()
{
    while (m_running)
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_cv.wait(lock, [this] { return !m_queue.empty() || !m_running; });

        while (!m_queue.empty())
        {
            LogMessage msg = std::move(m_queue.front());
            m_queue.pop();
            lock.unlock();

            printMessage(msg);

            lock.lock();
        }
    }

    // Flush remaining messages if any
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        if (m_queue.empty())
            break;
        LogMessage msg = std::move(m_queue.front());
        m_queue.pop();
        lock.unlock();

        printMessage(msg);
    }
}

void Logger::printMessage(const LogMessage& msg)
{
    // Format: | HH:MM:SS:ms:ns | LEVEL | Scope::Subscope: Message

    auto const duration_since_epoch = msg.time.time_since_epoch();
    auto const seconds_since_epoch =
        std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch);
    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        duration_since_epoch - seconds_since_epoch);
    auto const nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
        duration_since_epoch - seconds_since_epoch - milliseconds);
    auto const seconds_in_day = seconds_since_epoch.count() % 86400;

    auto const hours = static_cast<int>(seconds_in_day / 3600);
    auto const minutes = static_cast<int>((seconds_in_day % 3600) / 60);
    auto const seconds = static_cast<int>(seconds_in_day % 60);
    auto const ms = static_cast<int>(milliseconds.count());
    auto const ns = static_cast<int>(nanoseconds.count());

    auto const levelColor = getLogColor(msg.level);
    auto const reset = "\033[0m";
    auto const sepColor = getSeparatorColor();

    std::ostringstream oss;
    // clang-format off
    oss 
        << sepColor << "| " 
        << levelColor << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds << ":" << std::setw(3) << ms << ":" << std::setw(6) << ns
        << sepColor << " | " 
        << levelColor << std::setw(8) << std::setfill(' ') << std::right << getLogLevelName(msg.level)
        << sepColor << " | "
        << levelColor << msg.scope 
        << sepColor << " | "
        << reset << msg.message << "\n";
    // clang-format on

    {
        static std::mutex cout_mutex;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << oss.str();
    }
}

ScopeLogger::ScopeLogger(std::string_view const tag, std::string_view const scope)
    : m_tag{tag}, m_scope{scope}, m_start{std::chrono::high_resolution_clock::now()}
{
    static constexpr auto green = "\033[32m";
    static constexpr auto gray = "\033[90m";

    Logger::log(
        LogLevel::Scope, m_scope, "{}[{}+{}]{} Begin {}{}", gray, green, gray, green, green, m_tag);
}

ScopeLogger::~ScopeLogger()
{
    auto const end = std::chrono::high_resolution_clock::now();
    auto const elapsed = end - m_start;

    auto const hours = std::chrono::duration_cast<std::chrono::hours>(elapsed);
    auto const minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsed - hours);
    auto const seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed - hours - minutes);
    auto const milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(elapsed - hours - minutes - seconds);
    auto const nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
        elapsed - hours - minutes - seconds - milliseconds);

    static constexpr auto red = "\033[91m";
    static constexpr auto gray = "\033[90m";
    static constexpr auto reset = "\033[0m";

    std::ostringstream oss;
    oss << std::setfill('0');

    bool started = false;
    if (hours.count() > 0)
    {
        oss << hours.count() << "h";
        started = true;
    }
    if (started || minutes.count() > 0)
    {
        if (started)
        {
            oss << ", ";
        }
        oss << minutes.count() << "m";
        started = true;
    }
    if (started || seconds.count() > 0)
    {
        if (started)
        {
            oss << ", ";
        }
        oss << seconds.count() << "s";
        started = true;
    }
    if (started || milliseconds.count() > 0)
    {
        if (started)
        {
            oss << ", ";
        }
        oss << milliseconds.count() << "ms";
        started = true;
    }
    if (started || nanoseconds.count() > 0)
    {
        if (started)
        {
            oss << ", ";
        }
        oss << nanoseconds.count() << "ns";
    }
    oss << reset;

    Logger::log(
        LogLevel::Scope,
        m_scope,
        "{}[{}-{}]{} End {}{} {}~ elapsed {}",
        gray,
        red,
        gray,
        red,
        gray,
        m_tag,
        gray,
        oss.str());
}

} // namespace Graphite::Core::Logger
