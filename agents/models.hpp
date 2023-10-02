#pragma once
#include "agents.hpp"


// The abstrac model interface
class ModelBase
{
public:
    virtual AgentBase * get_agent( int idx ) = 0; // Use this to get an abstract representation of the agent at idx
};

/* Model<T> is a base class from which the acutal models would derive. They have efficient access to a vector of AgentT,
 * without any pointer indirections */
template<typename AgentT_>
class Model : public ModelBase
{
public:
    using AgentT = AgentT_;
    std::vector<AgentT> agents;
    Model( size_t n_agents )
    {
        agents = std::vector<AgentT>{ int( n_agents ), AgentT() };
    }

    AgentBase * get_agent( int idx ) override // For this to work AgentT needs to be a subclass of AgentBase
    {
        return &agents[idx];
    }
};

// Examples
class ModelVector : public Model<Agent<std::vector<double>>>
{
public:
    ModelVector() : Model( 2 )
    {
        agents = std::vector<AgentT>{ AgentT( { 0.0, 1.0 } ), AgentT( { 2.0, 3.0 } ) };
    }
};

class ModelDeGroot : public Model<Agent<double>>
{
public:
    ModelDeGroot() : Model( 2 )
    {
        agents = std::vector<AgentT>{ AgentT( 0.0 ), AgentT( 1.0 ) };
    }
};
