#ifndef CHRONO_H
#define CHRONO_H

#include <chrono>

constexpr std::uint64_t Time2ns(auto const& t)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
}

constexpr std::uint64_t Time2us(auto const& t)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
}

constexpr std::uint64_t Time2ms(auto const& t)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

constexpr std::uint64_t Time2s(auto const& t)
{
    return std::chrono::duration_cast<std::chrono::seconds>(t).count();
}

constexpr std::uint64_t Time2m(auto const& t)
{
    return std::chrono::duration_cast<std::chrono::minutes>(t).count();
}

inline auto Now(void)
{
    return std::chrono::high_resolution_clock::now();
}


#endif /* CHRONO_H */

