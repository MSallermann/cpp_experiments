#include <fmt/format.h>
#include <fmt/ranges.h>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <ranges>
#include <string_view>
#include <vector>

int main()
{
    auto isspace = []( unsigned char c ) { return std::isspace( c ); };

    {
        std::vector<int> dt = { 1, 4, 2, 3 };
        std::ranges::sort( dt.begin(), dt.end() );
        std::ranges::copy( dt.begin(), dt.end(), std::ostream_iterator<int>( std::cout, "," ) );
    }
    fmt::print( "\n" );
    {
        std::vector<int> dt = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::ranges::shuffle( dt, std::mt19937( std::random_device()() ) );
        auto pos = std::ranges::find( dt.begin(), std::unreachable_sentinel, 7 );
        std::ranges::copy( dt.begin(), ++pos, std::ostream_iterator<int>( std::cout, "," ) );
    }
    fmt::print( "\n" );
    {
        std::vector<int> dt = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::vector<int> result;
        std::ranges::transform(
            dt, dt | std::ranges::views::reverse, std::back_inserter( result ), std::minus<void>(),
            []( int v ) { return v * v; }, []( int v ) { return v * v; } );
        std::ranges::copy( result, std::ostream_iterator<int>( std::cout, "," ) );
    }
    fmt::print( "\n" );
    {
        std::string file_contents = "1  2.3 3.3 \n 1.5  3.0 2.9";

        auto split = std::ranges::split_view( file_contents, '\n' );
        for( auto line : split )
        {
            fmt::print( "line {}\n", line );

            std::cout << "--\n";
            auto columns = std::ranges::split_view( line, ' ' );
            double buffer;
            std::cout << "col ";

            for( auto c : columns )
            {
                if( std::isspace( c[0] ) )
                    continue;
                fmt::print( "column {}\n", c );
                // auto begin = std::ranges::find_if_not(c, isspace);
                // auto end = std::ranges::find_if_not(begin, c.end(), isspace);
                auto [ptr, ec] = std::from_chars( &( *c.begin() ), &( *c.end() ), buffer );
                fmt::print( "buffer = {}\n", buffer );
                // std::cout << buffer << "  ";
            }
            std::cout << "\n";
        }
    }
}
