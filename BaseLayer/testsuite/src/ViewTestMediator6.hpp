#ifndef _VIEWTESTMEDIATOR6_HPP_
#define _VIEWTESTMEDIATOR6_HPP_

#include "ViewTest.hpp"
#include "../../Component/Patterns/Observer/Notification.h"

#include <string>

namespace data
{
    using BaseLayer::Interfaces::INotification;
    using BaseLayer::Interfaces::IView;
    using BaseLayer::Patterns::Mediator;
    using BaseLayer::Core::View;
    /**
     * A Mediator class used by ViewTest.
     */
    struct ViewTestMediator6 : public Mediator
    {
        /**
         * The Mediator base name
         */
        static char const* const NAME;
                
        /**
         * Constructor
         */
        ViewTestMediator6(std::string const& name, const void* view) 
            :Mediator(name, view)
        { }

        virtual NotificationNames listNotificationInterests(void) const
        {
            // be sure that the mediator has some Observers created
            // in order to test removeMediator
            typedef StdContainerAggregate<std::list<std::string> > result_t;
            result_t* result = new result_t;
            result->get().push_back(ViewTest::NOTE6);
            return NotificationNames(result);
        }

        virtual void handleNotification(INotification const& notification)
        {
            (void)notification;
            IView& view = View::getInstance("ViewTestKey11");
            // Now supporting remove observer in list during loop
            view.removeMediator(getMediatorName());
        }

        void onRemove(void)
        {
            viewTest()->counter++;
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
    char const* const ViewTestMediator6::NAME = "ViewTestMediator6";
}

#endif //_VIEWTESTMEDIATOR6_HPP_
