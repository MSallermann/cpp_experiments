#include "agents/run.hpp"
#include "parse/run.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <argparse/argparse.hpp>
#include <string>

int main( int argc, char * argv[] )
{

    argparse::ArgumentParser program( "cpp_experiments" );

    program.add_argument( "experiment" ).help( "The experiment to be run." );

    try
    {
        program.parse_args( argc, argv );
    }
    catch( const std::runtime_error & err )
    {
        fmt::print( stderr, "{}\n{}", err.what(), fmt::streamed( program ) );
        return 1;
    }

    auto experiment = program.get<std::string>( "experiment" );

    if( experiment == "agents" )
    {
        Agents::run();
    }
    else if (experiment == "parse")
    {
        Parse::run();
    }
    else
    {
        throw std::runtime_error( "Experiment string not known" );
    }
}