#include <string>
#include <iostream>

#include "./test_controller.cpp"


#define GET_VAR_NAME(variable) #variable


void run_test(std::string name, bool (*func)())
{
    std::cout << name;
    std::cout << "...";
    bool result = func();
    if (result){
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

int main()
{
    run_test(GET_VAR_NAME(test_controller), test_controller);
    return 0;
}

