#include <iostream>
#include <type_traits>

template <typename N, typename F>
void doOperation( F f)
{
    N neigh;
    
    int temp = neigh();
    f(temp);
    std::cout << "Result is " << temp << std::endl;
}

struct Neighbor {
    int operator() () { return 42; }
};

struct Filter {
    int value_;

    void operator() (int &v) {v+= value_;}
};

int main()
{
  // prints 47
  doOperation<Neighbor>( Filter{5} );
}
