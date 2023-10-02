#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <random>

std::vector<size_t> reservoir_sample( size_t n, size_t k, std::mt19937 & mt )
{
    std::uniform_real_distribution<double> distribution( 0.0, 1.0 );
    std::uniform_int_distribution<size_t> distribution_int( 0, k - 1 );

    std::vector<size_t> reservoir( k );

    // Fill the reservoir
    for( size_t i = 0; i < k; i++ )
    {
        reservoir[i] = double( i );
    }

    double W = std::exp( std::log( distribution( mt ) ) / k );

    size_t i = k - 1;
    while( i < n )
    {
        i += std::floor( std::log( distribution( mt ) ) / std::log1p( -W ) ) + 1;
        if( i < n )
        {
            // replace a random item of the reservoir with item i
            auto const idx = distribution_int( mt );
            reservoir[idx] = i;
            W *= std::exp( std::log( distribution( mt ) ) / k );
        }
    }
    return reservoir;
}

std::vector<size_t> reservoir_sample_std( size_t n, size_t k, std::mt19937 & mt )
{
    std::vector<size_t> reservoir( k );
    std::vector<size_t> population( n );
    std::sample( population.begin(), population.end(), reservoir.begin(), k, mt );
    return reservoir;
}

std::vector<size_t> reservoir_sample_std_iterator( size_t n, size_t k, std::mt19937 & mt )
{
    std::vector<size_t> reservoir( k );

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = size_t;
        using pointer           = size_t *; // or also value_type*
        using reference         = size_t &;

        Iterator( size_t i ) : i( i ) {}
        size_t i;
        size_t & operator*()
        {
            return i;
        };
        bool operator==( const Iterator & it1 )
        {
            return i == it1.i;
        };
        Iterator & operator++()
        {
            i++;
            return *this;
        }
    };

    std::sample( Iterator( 0 ), Iterator( n ), reservoir.begin(), k, mt );
    return reservoir;
}

template<typename func_t>
size_t benchmark( func_t f, size_t times, size_t k, size_t n, std::mt19937 & mt )
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    for( size_t t = 0; t < times; t++ )
    {
        f( n, k, mt );
    }

    auto t2     = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>( t2 - t1 );
    return ms_int.count();
}

int main()
{
    std::random_device rd = std::random_device();
    auto mt               = std::mt19937( rd() );

    size_t n  = 10;
    size_t k  = 4;
    auto res  = reservoir_sample( n, k, mt );
    auto res2 = reservoir_sample_std( n, k, mt );

    auto n_tries = 500;

    std::fstream output;
    output.open( "./output.txt", std::fstream::in | std::fstream::out | std::fstream::trunc );

    fmt::print( output, "method k n time\n" );

    for( size_t n : { 5000, 7500, 10000, 12500, 15000, 17500, 20000, 25000, 30000 } )
    {
        for( size_t k : { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 } )
        {
            auto time = benchmark( reservoir_sample, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample", k, n, time );
            time = benchmark( reservoir_sample_std, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample_std", k, n, time );
            time = benchmark( reservoir_sample_std_iterator, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample_std_iterator", k, n, time );
        }
    }

    // larger system
    for( size_t n : { 100000 } )
    {
        for( size_t k = 0; k < n; k += 1000 )
        {
            auto time = benchmark( reservoir_sample, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample", k, n, time );
            time = benchmark( reservoir_sample_std, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample_std", k, n, time );
            time = benchmark( reservoir_sample_std_iterator, n_tries, k, n, mt );
            fmt::print( output, "{} {} {} {}\n", "reservoir_sample_std_iterator", k, n, time );
        }
    }

    output.close();
    return 0;
}