#ifndef _CONTROLLERTESTHERITANCE_HPP_
#define _CONTROLLERTESTHERITANCE_HPP_

#include "../../Core/Controller/Controller.h"

namespace data
{
    using BaseLayer::Core::Controller;

    class ControllerTestInheritance 
        : public Controller
    {
        friend class Controller;
    public:
        std::string* initializeControllerCalled;
    protected:
        ControllerTestInheritance(void)
            :Controller(this, "ControllerTestInheritance")
        {
        }
    public:
        static ControllerTestInheritance& getInstance(void)
        {
            if (Controller::find("ControllerTestInheritance") == NULL)
                new ControllerTestInheritance();
            return *(dynamic_cast<ControllerTestInheritance*>(&Controller::getInstance("ControllerTestInheritance")));
        }
    protected:
        virtual void initializeController(void)
        {
            initializeControllerCalled = new std::string("Called!!!");
            Controller::initializeController();
        }
    public:
        ~ControllerTestInheritance()
        {
            delete initializeControllerCalled;
        }
    };
}

#endif /* __CONTROLLER_TEST_HERITANCE_HPP__ */
