#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <any>
#include <iostream>
#include <memory>
#include <vector>

// AGENTS
/* Agent Base is the abstract interface, for every part of the code that is not the actual computation, IO etc. */
class AgentBase
{
public:
    virtual std::string to_string() const = 0;

    // TODO: eventually a from_string might also be needed
    // virtual void from_string()          = 0;
};

/* Agent<T> is a base class for actual agents with opinion type T, it needs to implement to_string and from_string*/
template<typename T>
class Agent : public AgentBase
{
public:
    using opinion_t = T;
    opinion_t opinion;
    Agent() = default;
    Agent( opinion_t opinion ) : opinion( opinion ) {}

    // virtual void from_string() override
    // {
    // }

    virtual std::string to_string() const override
    {
        return fmt::format( "{}", opinion );
    }

    ~Agent() = default;
};

// If format cannot generate the to_string function on its own, we have to specialize the template
// Note: most likely we would have to do the same for from_string for most values
struct TwoInt
{
    int a = 2;
    int b = 3;
};
template<>
inline std::string Agent<TwoInt>::to_string() const
{
    return fmt::format( "a = {}, b = {}", opinion.a, opinion.b );
};
