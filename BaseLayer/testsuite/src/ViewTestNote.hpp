#ifndef _VIEWTESTNOTE_HPP_
#define _VIEWTESTNOTE_HPP_

#include "../../Component/Patterns/Observer/Notification.h"

#include <string>

namespace data
{
    using BaseLayer::Patterns::Notification;
   /**
   * A Notification class used by ViewTest.
   * 
   * @see org.puremvc.as3.multicore.core.view.ViewTest ViewTest
   */
    struct ViewTestNote : public Notification
    {
        /**
         * The name of this Notification.
         */
        static char const* const NAME;
        /**
         * Constructor.
         * 
         * @param name Ignored and forced to NAME.
         * @param body the body of the Notification to be constructed.
         */
        ViewTestNote(std::string const& name, void const* body)
            :Notification(name, body)
        { }
        
        /**
         * Factory method.
         * 
         * <P> 
         * This method creates new instances of the ViewTestNote class,
         * automatically setting the note name so you don't have to. Use
         * this as an alternative to the constructor.</P>
         * 
         * @param name the name of the Notification to be constructed.
         * @param body the body of the Notification to be constructed.
         */
        static INotification* create(std::string const& name, void const* body ) 
        {
            return new data::ViewTestNote(name, body);
        }
    };
    char const* const ViewTestNote::NAME = "ViewTestNote";
}

#endif //_VIEWTESTNOTE_HPP_
