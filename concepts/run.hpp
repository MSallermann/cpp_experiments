#pragma once
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <array>
#include <iterator>
#include <vector>

namespace Concepts
{

template<typename Container>
requires std::contiguous_iterator<typename Container::iterator>
void bla( Container cont )
{
    auto a          = cont.begin();
    auto b          = cont.end();
    auto size_bytes = std::distance( a, b ) * sizeof( decltype( cont[0] ) );
    void * buffer   = &a[0];
    fmt::print( "Size in bytes {}, buffer {}\n", size_bytes, buffer );
}

template<typename Container>
requires std::contiguous_iterator<typename Container::iterator> 
void bla( Container cont, bool probe )
{
    auto a          = cont.begin();
    auto b          = cont.end();
    auto size_bytes = std::distance( a, b ) * sizeof( decltype( cont[0] ) );
    void * buffer   = &a[0];
    fmt::print( "Size in bytes {}, buffer {}\n", size_bytes, buffer );
}

void run()
{
    std::array<double, 5> arr;
    std::vector<float> vec( 10 );

    bla( arr );
    bla( vec );

    // std::vector<float>::iterator;
}

} // namespace Concepts