#include <chrono>
#include <vector>
#include <fmt/format.h>

class A
{
public:
    std::vector<double> v;

    A( std::vector<double> A_ ) : v( A_ ) {}
    // A( std::vector<double> && A ) : v(A) {}
};

class A2
{
public:
    std::vector<double> v;

    A2( std::vector<double> && A_ ) : v( A_ ) {}
    // A( std::vector<double> && A ) : v(A) {}
};

int main()
{
    int N    = 1e6;
    auto vec = std::vector<double>( N );
    auto vec2 = std::vector<double>( N );

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    auto A1 = A(vec);

    auto t2     = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>( t2 - t1 );
    fmt::print( "{}\n", ms_int.count() );

    auto a2 = A2( std::move( vec2 ) );

    auto t3 = high_resolution_clock::now();

    ms_int = duration_cast<milliseconds>( t3 - t2 );
    fmt::print( "{}\n", ms_int.count() );
    return 0;
}
