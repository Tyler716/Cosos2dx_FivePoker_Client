#ifndef _FACADETESTINHERITANCE_HPP_
#define _FACADETESTINHERITANCE_HPP_

#include "../../Component/Patterns/Facade/Facade.h"

namespace data
{
    using BaseLayer::Interfaces::IFacade;
    using BaseLayer::Patterns::Facade;

    class FacadeTestInheritance
        : public Facade
    {
        friend class Facade;

    public:
        std::string* initializeNotifierCalled;
        std::string* initializeFacadeCalled;

    protected:
        FacadeTestInheritance(void)
            : Facade(this, "FacadeTestInheritance")
        {
        }

    public:
        static FacadeTestInheritance& getInstance(void)
        {
            if (Facade::hasCore("FacadeTestInheritance"))
            {
                return *(dynamic_cast<FacadeTestInheritance*>(&Facade::getInstance("FacadeTestInheritance")));
            }
            return *(new FacadeTestInheritance());
        }

    protected:
        void initializeNotifier(std::string const& key)
        {
            initializeNotifierCalled = new std::string("Called!!!");
            Facade::initializeNotifier(key);
        }

        void initializeFacade(void)
        {
            initializeFacadeCalled = new std::string("Called!!!");
            Facade::initializeFacade();
        }
        ~FacadeTestInheritance()
        {
            delete initializeNotifierCalled;
            delete initializeFacadeCalled;
        }
    };
}

#endif //_FACADETESTINHERITANCE_HPP_
