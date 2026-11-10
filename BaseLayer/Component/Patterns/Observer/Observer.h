#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "../../Interfaces/IObserver.h"
#include "../../Interfaces/INotification.h"

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::IObserver;
        using Interfaces::INotification;

        template<typename _Method, typename _Context>
        class Observer : public IObserver
        {
        private:
            Observer(void);

        public:
            /**
             * Copy constructor
             */
            explicit Observer(Observer const &arg)
                : IObserver()
                , _notify_method(arg._notify_method)
                , _notify_context(arg._notify_context)
            {
            }

        public:
            /**
             * Constructor.
             *
             * <P>
             * The notification method on the interested object should take
             * one parameter of type <code>INotification</code></P>
             *
             * @param notify_method the notification method of the interested object
             * @param notify_context the notification context of the interested object
             */
            explicit Observer(_Method notify_method, _Context *notify_context)
                : IObserver()
                , _notify_method(notify_method)
                , _notify_context(notify_context)
            {
            }

            /**
             * Notify the interested object.
             *
             * @param notification the <code>INotification</code> to pass to the interested object's notification method.
             */
            virtual void notifyObserver(INotification const &notification)
            {
                if (_notify_context == NULL)
                {
                    //throw std::runtime_error("Notify context is null.");
                }

                if (_notify_method == NULL)
                {
                    //throw std::runtime_error("Notify method is null.");
                }

                (*_notify_context.*_notify_method)(notification);
            }

            /**
             * Compare an object to the notification context.
             *
             * @param object the object to compare.
             * @return boolean indicating if the object and the notification context are the same.
             */
            virtual bool compareNotifyContext(void const *object) const
            {
                return _notify_context == object;
            }

            /**
             * Copy operator.
             */
            Observer &operator=(Observer const &arg)
            {
                _notify_method = arg._notify_method;
                _notify_context = arg._notify_context;
                return *this;
            }

            /**
             * Virtual destructor.
             */
            virtual ~Observer(void)
            {
            }

        private:
            _Method _notify_method;
            _Context *_notify_context;
        };

        /**
         * Create new observer.
         */
        template<typename _Method, typename _Context> inline IObserver* createObserver(
            _Method notify_method, _Context* notify_context)
        {
            return new Observer<_Method, _Context>(notify_method, notify_context);
        }

        /**
         * Make an observer.
         */
        template<typename _Method, typename _Context> inline Observer<_Method, _Context> makeObserver(
            _Method notify_method, _Context* notify_context)
        {
            return Observer<_Method, _Context>(notify_method, notify_context);
        }
    }
}

#endif //_OBSERVER_H_
