#include <iostream>
#include <vector>
#include <concepts>

template <typename T, typename U>
bool bar ( T s, U g ) {
    return s == g;
}


template<typename F, typename T>
void foo ( F f, T s , T g ) {
    std::cout << std::boolalpha << f(s,g) << std::endl;
}

template<typename F, typename T>
requires std::equivalence_relation<F, T, T>
void foo_constrained ( F f, T s , T g ) {
    std::cout << std::boolalpha << f(s,g) << std::endl;
}

int main()
{
    {
        auto l = [](int s, int g) -> bool { return s==g; };  

        foo( l, 1 , 1 ); // true
        foo( l, 1 , 0 ); // false

        foo (bar<std::string, std::string>, "aaa", "bbb"); // false

    }

    {
        auto l = []() -> bool {return true;};
        // foo(l, 1, 0); // compilation error; 
        // foo_constrained (l, 1, 0); // compilation errror; constrained unsutisfied
    }

    {
        auto l = [](int s, int g ) -> bool {return true;};
        foo(l, 1, 0); // true 
        foo_constrained (l, 1, 0); // true
    }


    return 0;
}
