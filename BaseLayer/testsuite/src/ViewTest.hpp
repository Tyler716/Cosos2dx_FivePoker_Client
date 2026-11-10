#ifndef _VIEWTEST_HPP_
#define _VIEWTEST_HPP_

#include "../../Component/Interfaces/INotification.h"
#include "../../Component/Patterns/Command/Command.h"
#include "../../Component/Patterns/Observer/Observer.h"

#include <string>
#include <map>
    
namespace data
{
    using BaseLayer::Interfaces::INotification;

    struct ViewTest
    {
        std::string lastNotification;
        bool onRegisterCalled;
        bool onRemoveCalled;
        int counter;

        std::map<std::string, int> viewTestVars;
        ViewTest();
        // viewTestMethod must be virtual!!! 
        virtual void viewTestMethod(INotification const& note);
        static char const* const NOTE1;
        static char const* const NOTE2;
        static char const* const NOTE3;
        static char const* const NOTE4;
        static char const* const NOTE5;
        static char const* const NOTE6;

        virtual ~ViewTest(){};
    };
}

#endif //_VIEWTEST_HPP_
