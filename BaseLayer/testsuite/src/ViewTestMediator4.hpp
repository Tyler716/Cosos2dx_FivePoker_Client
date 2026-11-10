#ifndef _VIEWTESTMEDIATOR4_HPP_
#define _VIEWTESTMEDIATOR4_HPP_

#include "ViewTest.hpp"

#include <string>

namespace data
{
    using BaseLayer::Patterns::Mediator;
    /**
     * A Mediator class used by ViewTest.
     */
    struct ViewTestMediator4 : public Mediator
    {
       /**
         * The Mediator name
         */
        static char const* const NAME;
                
        /**
         * Constructor
         */
        ViewTestMediator4(const void* view) 
            :Mediator(NAME, view)
        { }

        void onRegister(void)
        {
            viewTest()->onRegisterCalled = true;
        }

        void onRemove(void)
        {
            viewTest()->onRemoveCalled = true;
        }

        ViewTest const* viewTest(void) const
        {
            return (ViewTest*)_view_component;
        }

        ViewTest* viewTest(void)
        {
            return (ViewTest*)_view_component;
        }
    };
    char const* const ViewTestMediator4::NAME = "ViewTestMediator4";
}

#endif //_VIEWTESTMEDIATOR4_HPP_
