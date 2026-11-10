#include "../include/tut/tut.hpp"
#include "../include/tut/tut_console_reporter.hpp"
#include "../include/tut/tut_cppunit_reporter.hpp"

#include <iostream>
#include <string.h>

int main(int argc, char *argv[])
{
    tut::console_reporter *reporter;
    tut::cppunit_reporter unit_reporter;
    tut::test_result result;
    reporter = new tut::console_reporter;

    tut::runner.get().set_callback(reporter);
    tut::runner.get().insert_callback(&unit_reporter);

    // Check command line options.
    switch(argc)
    {
        case 1: // No options: run all tests.
            std::cout << "--> Run all tests" << std::endl;
            tut::runner.get().run_tests();
            std::cout << "--> Finished" << std::endl;
            break;

        default:    // Parameters error.
            std::cerr<<"incorrect number of arguments. usage:"<<std::endl;
            std::cerr<<"  "<<argv[0]<<std::endl;
            std::cerr<<"  "<<argv[0]<<" <test_group_name>"<<std::endl;
            std::cerr<<"  "<<argv[0]<<" <test_group_name> <test_case_number>"<<std::endl;
            std::cin.get();
            return 2;
            break;      // Never rech here.
    }

    int returnValue = ( reporter->all_ok() ) ? 0 : 1;

    delete reporter;

    std::cin.get();

    return returnValue;   // Invert logic for shell.
}
