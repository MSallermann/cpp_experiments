#include "models.hpp"

int main()
{
    std::unique_ptr<ModelBase> model = std::make_unique<ModelVector>( ModelVector() );
    auto var                         = model->get_agent( 0 )->to_string();
    std::cout << var << "\n";
    var = model->get_agent( 1 )->to_string();
    std::cout << var << "\n";

    model = std::make_unique<ModelDeGroot>( ModelDeGroot() );
    var   = model->get_agent( 0 )->to_string();
    std::cout << var << "\n";
    var = model->get_agent( 1 )->to_string();
    std::cout << var << "\n";

    using ModelTwoInt = Model<Agent<TwoInt>>;

    model = std::make_unique<ModelTwoInt>( ModelTwoInt( 2 ) );
    var   = model->get_agent( 0 )->to_string();
    std::cout << var << "\n";
    var = model->get_agent( 1 )->to_string();
    std::cout << var << "\n";

    return 0;
}
