#include <iostream>

#include <queue>
#include <stack>
#include <concepts>

template <typename Container>
concept ContainerWithFront = requires (Container c) {{c.front()} -> std::convertible_to <typename Container::value_type>; };

template <typename Container>
concept ContainerWithTop = requires (Container c) {{c.top()} -> std::convertible_to <typename Container::value_type>; };


template <typename Container>                                                                                      
concept ContainerWithOrder = (ContainerWithTop<Container> || ContainerWithFront<Container> ) && 
                              requires (Container c, typename Container::value_type const& el)                      
                                        { c.push(el); c.pop();};
                      
template <ContainerWithFront Q>
Q::value_type front_imp(Q const& q) {
    return q.front();
}

template <ContainerWithTop Q>
Q::value_type front_imp(Q const& q) {
    return q.top();
}


template <ContainerWithOrder Q>
void foo () {
    Q q;

    q.push(1);
    q.push(2);
    q.push(3);

    while (!q.empty()) {
        std::cout << front_imp(q) << std::endl;
        q.pop();
    }
}

int main (int argc, char** argv) {

    foo<std::queue<int>>();
    std::cout << "-----\n";
    foo<std::priority_queue<int>>();
    std::cout << "-----\n";
    foo<std::stack<int>>();

    foo<std::vector<int>>();

}
