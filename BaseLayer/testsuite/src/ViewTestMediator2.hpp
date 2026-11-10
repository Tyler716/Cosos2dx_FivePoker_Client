#ifndef _VIEWTESTMEDIATOR2_HPP_
#define _VIEWTESTMEDIATOR2_HPP_

#include "ViewTest.hpp"
#include "../../Component/Patterns/Observer/Notification.h"

#include <string>

namespace data
{
    using BaseLayer::Patterns::Mediator;
    /**
     * A Mediator class used by ViewTest.
     */
    struct ViewTestMediator2 : public Mediator
    {
       /**
         * The Mediator name
         */
        static char const* const NAME;
                
        /**
         * Constructor
         */
        ViewTestMediator2(const void* view) 
            :Mediator(NAME, view)
        { }

        virtual NotificationNames listNotificationInterests(void) const
        {
            // be sure that the mediator has some Observers created
            // in order to test removeMediator
            typedef StdContainerAggregate<std::list<std::string> > result_t;
            result_t* result = new result_t;
            result->get().push_back(ViewTest::NOTE1);
            result->get().push_back(ViewTest::NOTE2);
            return NotificationNames(result);
        }

        virtual void handleNotification(INotification const& notification)
        {
            viewTest()->lastNotification = notification.getName();
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
    char const* const ViewTestMediator2::NAME = "ViewTestMediator2";
}

#endif //_VIEWTESTMEDIATOR2_HPP_
