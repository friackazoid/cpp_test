#include <iostream>
#include <type_traits>

template <size_t N>
struct fact :
    std::integral_constant<size_t, N * fact<N - 1>{}> {};

template<>
struct fact<0> :
    std::integral_constant<size_t, 1> {};

constexpr int foo () {return 1;}

int main (int argc, char **argv) {

    constexpr int i = 5;
    std::cout << "factorial " << i << " = " << fact<i>{} << std::endl;

    // FAIL
    //std::cout << "factorial '5.5' " << fact<5.5>{} << std::endl;
    std::cout << "factorial foo = " << fact< foo()  >{} << std::endl;

    // out 99
    std::cout << "sc: " << static_cast<size_t>( 'c'  ) << std::endl;

    // fact<99> type overflow result 0
    std::cout << "factorial 'c' = " << fact<'c'>{} << std::endl;

    return 0;
}
