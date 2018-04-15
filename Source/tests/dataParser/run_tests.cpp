#include <string>
#include <iostream>

#include "./test_insert.cpp"
#include "./test_update.cpp"
#include "./test_delete.cpp"
#include "./test_saveload.cpp"

#define GET_VAR_NAME(variable) #variable


void run_test(std::string name, bool (*func)())
{
    std::cout << name;
    std::cout << "...";
    bool result = func();
    if (result) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

int main()
{
    // INSERT tests
    run_test(GET_VAR_NAME(test_insert), test_insert);

    // UPDATE tests
    run_test(GET_VAR_NAME(test_update), test_update);

    // DELETE tests
    run_test(GET_VAR_NAME(test_delete), test_delete);

    // SAVE + LOAD tests
    //run_test(GET_VAR_NAME(test_saveload), test_saveload);

    return 0;
}

