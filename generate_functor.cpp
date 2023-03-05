
// g++ -std=c++20 -o generate_functor generate_functor.cpp

#include <iostream>
#include <vector>
#include <algorithm>

void print_v (std::string const& annotation, std::vector<std::vector<int>> const& g) {
    
    for (std::cout  << annotation <<  std::endl; const auto& v : g) {
        for (const auto& i: v) {
            std::cout << i << " "; 
        }
        std::cout << std::endl;
    }  
}

template <size_t N>
struct FooGenerator {
    int operator() () { static int i_ = N; return i_++; }
};

int main()
{
    int k = 3;

    // Prints
    // 0 1 2 
    // 3 4 5 
    // 6 7 8 
    // as expected
    {
        std::vector<std::vector<int>> g(k);
    
        for ( auto n{0}; auto& v : g) {
            v.resize(k);
            std::generate(v.begin(), v.end(), [&n]() {return n++;} );
        }
    
        print_v( "Generate with lambda" , g);
    }

    // Prints
    // 0 1 2 
    // 0 1 2 
    // 0 1 2 
    // as genarete copy f in every loop
    {
        struct Foo {
            int i_ {0};

            Foo () = default;
            Foo (Foo const& foo) : i_(foo.i_)  { std::cout << "Foo::copy ctor; i_ = " << i_ << std::endl;}

            int operator() () { return i_++; }
        } f;

        std::vector<std::vector<int>> g(k);

        for ( auto& v : g) {
            v.resize(k);
            std::generate(v.begin(), v.end(), f );
        }
    
        print_v( "Generate with Foo functor (copy)" , g);
    }
    
    // Prints
    // 0 1 2 
    // 3 4 5 
    // 6 7 8 
    {

        const size_t start = 0;
        
        std::vector<std::vector<int>> g(k);
        FooGenerator<start> f;

        for ( auto& v : g) {
            v.resize(k);
            std::generate(v.begin(), v.end(), f );
        }
    
        print_v( "Generate with FooGenerator " , g);
    }
    
    return 0;
}

