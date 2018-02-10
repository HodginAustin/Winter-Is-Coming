#include <string>
#include <iostream>

#include "./test_controller.cpp"
#include "./test_dailystate.cpp"
#include "./test_profile.cpp"
#include "./test_zone.cpp"
#include "./test_internal_state_manager.cpp"
#include "./test_data_parser.cpp"


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
    // Internal state tests
    run_test(GET_VAR_NAME(test_internal_state_manager), test_internal_state_manager);
    run_test(GET_VAR_NAME(test_controller), test_controller);
    run_test(GET_VAR_NAME(test_dailystate), test_dailystate);
    run_test(GET_VAR_NAME(test_profile), test_profile);
    run_test(GET_VAR_NAME(test_zone), test_zone);

    // Data parser tests
    run_test(GET_VAR_NAME(test_data_parser), test_data_parser);

    return 0;
}

