#include <iostream>
#include <vector>

class Foo {
  public:
    std::vector< std::vector<int> > v_;
    
    Foo ( std::vector< std::vector<int> > const& v ) : v_(v) {
        
        std::cout << "Ctor Foo" << std::endl;
        
        for (auto vi : v_) {
            for (auto i : vi) {
                std::cout << i << " ";
            }
            
            std::cout << std::endl;
        }
    }
    
    // unexpected behavior
    // accidentally setting the return type to int instead of void
    // cause the program to freeze 
    void operator() () {
        std::cout << std::endl << "operator Foo" << std::endl;
        for (auto vi : v_) {
            for (auto i : vi) {
                std::cout << i << " ";
            }
            
            std::cout << std::endl;
        }
    }
};

int main()
{
  std::vector< std::vector<int> > v {
      {0,1,2},
      {3,4,5}};

    Foo vc(v);
    vc();
    
    return 0;
}
