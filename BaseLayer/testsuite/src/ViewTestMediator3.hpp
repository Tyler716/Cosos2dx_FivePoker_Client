#ifndef _VIEWTESTMEDIATOR3_HPP_
#define _VIEWTESTMEDIATOR3_HPP_

#include "ViewTest.hpp"
#include "../../Component/Patterns/Observer/Notification.h"

#include <string>

namespace data
{
    using BaseLayer::Patterns::Mediator;
    /**
     * A Mediator class used by ViewTest.
     */
    struct ViewTestMediator3 : public Mediator
    {
       /**
         * The Mediator name
         */
        static char const* const NAME;
                
        /**
         * Constructor
         */
        ViewTestMediator3(const void* view) 
            :Mediator(NAME, view)
        { }

        virtual NotificationNames listNotificationInterests(void) const
        {
            // be sure that the mediator has some Observers created
            // in order to test removeMediator
            typedef StdContainerAggregate<std::list<std::string> > result_t;
            result_t* result = new result_t;
            result->get().push_back(ViewTest::NOTE3);
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
    char const* const ViewTestMediator3::NAME = "ViewTestMediator3";
}

#endif //_VIEWTESTMEDIATOR3_HPP_
