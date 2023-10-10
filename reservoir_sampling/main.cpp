#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <queue>
#include <random>
#include <utility>

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

template<typename WeightCallbackT>
std::vector<size_t> reservoir_sampling_A_ExpJ( size_t n, size_t k, WeightCallbackT weight, std::mt19937 & mt )
{
    std::uniform_real_distribution<double> distribution( 0.0, 1.0 );

    std::vector<size_t> reservoir( k );
    using QueueItemT = std::pair<size_t, double>;

    auto compare = []( const QueueItemT & item1, const QueueItemT & item2 ) { return item1.second > item2.second; };
    std::priority_queue<QueueItemT, std::vector<QueueItemT>, decltype( compare )> H;

    size_t idx = 0;
    while( idx < n & H.size() < k )
    {
        double r = std::pow( distribution( mt ), 1.0 / weight( idx ) );
        H.push( { idx, r } );
        idx++;
    }

    auto X = std::log( distribution( mt ) ) / std::log( H.top().second );
    while( idx < n )
    {
        auto w = weight( idx );
        X -= w;
        if( X <= 0 )
        {
            auto t                     = std::pow( H.top().second, w );
            auto uniform_from_t_to_one = distribution( mt ) * ( 1.0 - t ) + t; // Random number in interval [t, 1.0]
            auto r                     = std::pow( uniform_from_t_to_one, 1.0 / w );
            H.pop();
            H.push( { idx, r } );
            X = std::log( distribution( mt ) ) / std::log( H.top().second );
        }
        idx++;
    }

    std::vector<size_t> result( H.size() );

    for( int i = 0; i < k; i++ )
    {
        // fmt::print( "{}: {}, {}\n", i, H.top().first, H.top().second );
        result[i] = H.top().first;
        H.pop();
    }

    return result;
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

void benchmark_draw_k_from_n()
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
}

void test_expJ()
{
    const size_t n = 1000;
    const size_t k = 800;

    std::random_device rd = std::random_device();
    auto mt               = std::mt19937( rd() );

    // auto WeightFunc = []( size_t idx ) { return std::abs( n / 2.0 - idx ); };
    auto WeightFunc = []( size_t idx ) { return double(idx); };
    // auto WeightFunc = []( size_t idx ) { return std::sin(idx/100.0) + 1.0; };

    const size_t n_tries = 10000;

    std::fstream output;
    output.open( "./output_a_expj.txt", std::fstream::in | std::fstream::out | std::fstream::trunc );

    for( size_t i = 0; i < n_tries; i++ )
    {
        auto res = reservoir_sampling_A_ExpJ( n, k, WeightFunc, mt );
        for( const auto & i : res )
        {
            fmt::print( output, "{} ", i );
        }
        fmt::print( output, "\n" );
    }

    output.close();
}

int main()
{
    // benchmark_draw_k_from_n();
    test_expJ();
    return 0;
}