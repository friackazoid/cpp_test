// --std=c++20 -O3

#include <iostream>
#include <vector>
#include <concepts>


template <typename T>
requires std::equality_comparable<T>
bool foo(T const& a, T const& b) {return a==b;}

int main()
{


    std::cout << std::boolalpha  << foo(2, 3) << std::endl; //false
    std::cout << std::boolalpha  << foo(2, 2) << std::endl; // true
    std::cout << std::boolalpha  << foo(std::make_pair(1,1), std::make_pair(0,0)) << std::endl; // false
    std::cout << std::boolalpha  << foo(std::make_pair(1,1), std::make_pair(1,1)) << std::endl; // true

    {
        std::vector<int> a{1,2,3}, b{1,2,3}, c{0} ;
        std::cout << std::boolalpha << "std::vector == std::vector " << foo(a, b) << std::endl; // true
        std::cout << std::boolalpha << "std::vector == std::vector " << foo(a, c) << std::endl; //false
    }

    {
        char a[] = "aa", b[] = "aa"; 
        std::cout << std::boolalpha  << "a[] == b[] " << foo(a, b) << std::endl; // !!! 
    }

    return 0;
}
