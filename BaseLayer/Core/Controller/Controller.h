#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// STL include
#include <string>
#include <map>

#include "../../Component/Interfaces/IController.h"
#include "../../Component/Interfaces/ICommand.h"
#include "../../Component/Interfaces/IView.h"
#include "../../Component/Interfaces/INotification.h"

namespace BaseLayer
{
    namespace Core
    {
        using Interfaces::IController;
        using Interfaces::ICommand;
        using Interfaces::IView;
        using Interfaces::INotification;

        class Controller : public virtual IController
        {
        protected:
            typedef std::map<std::string, ICommand*> CommandMap;

        protected:
            /**
             * Define iterator converter for getting notification name only.
             */
            struct IteratorConverter
                : public std::unary_function<CommandMap::const_iterator, CommandMap::key_type>
            {
                /**
                 * Converting operator.
                 *
                 * @param iterator the iterator of internal container.
                 * @return the name of notification inside iterator.
                 */
                CommandMap::key_type const &operator()(CommandMap::const_iterator const &iterator) const;
            };

            /**
             * Define iterator range of internal container.
             */
            struct IteratorRange
            {
                /**
                 * Get begin point.
                 *
                 * @return the begin of iterator.
                 */
                CommandMap::const_iterator begin(CommandMap const *const &iterator) const;

                /*
                 * Get end point.
                 *
                 * @return the end of iterator.
                 */
                CommandMap::const_iterator end(CommandMap const *const &iterator) const;
            };

        public:
            static char const *const DEFAULT_KEY;

        protected:
            // Message Constants
            static char const *const MULTITON_MSG;

            // The Multiton Key for this Core
            std::string const _multiton_key;

            // Local reference to View
            IView *_view;

            // Mapping of Notification names to Command Class references
            CommandMap _command_map;

        private:
            //Copy constructor
            Controller(Controller const&);
            Controller(IController const&);
            Controller &operator=(Controller const&);
            Controller &operator=(IController const&);

        protected:
            static IController *find(std::string const &key);
            static void insert(std::string const &key, IController *controller);

        public:
            /**
             * Constructor.
             *
             * <P>
             * This <code>IController</code> implementation is a Multiton,
             * so you should not call the constructor
             * directly, but instead call the static Factory method,
             * passing the unique key for this instance
             * <code>Controller::getInstance( key )</code>
             *
             * @throws Error Error if instance for this Multiton key has already been constructed
             *
             */
            explicit Controller(std::string const &key = Controller::DEFAULT_KEY);

        protected:
            // Support call virtual method in constructor of base class (Controller).
            // Condition: Derived class (class inherit from Controller) must implement
            // method: initializeController
            /**
             * Constructor.
             *
             * <P>
             * Support call virtual method in constructor of base class (<code>Controller</code>).
             *
             * Condition: Derived class (class inherit from <code>Controller</code>)
             * must implement method: initializeController
             *
             * Please use in this way:
             * <code>
             *
             * ConcreateController::ConcreateController(void)
             * :Controller(this)
             * {
             *     // Your code here
             * }
             * </code>
             *
             * Factory method <code>Controller::getInstance( key )</code>
             *
             * @throws Error Error if instance for this Multiton key has already been constructed
             *
             */
            template<typename _DerivedType> explicit Controller(
                _DerivedType *instance, std::string const &key = Controller::DEFAULT_KEY)
                : _multiton_key(key)
                , _view(NULL)
            {
                if (find(_multiton_key))
                {
                    //TODO:add error log
                    //throw std::runtime_error(MULTITON_MSG);
                }

                insert(_multiton_key, this);
                instance->_DerivedType::initializeController();
            }

            /**
             * Initialize the Multiton <code>Controller</code> instance.
             *
             * <P>Called automatically by the constructor.</P>
             *
             * <P>Note that if you are using a subclass of <code>View</code>
             * in your application, you should <i>also</i> subclass <code>Controller</code>
             * and override the <code>initializeController</code> method in the
             * following way:</P>
             *
             * <code>
             *        // ensure that the Controller is talking to my IView implementation
             *        override public function initializeController(  ) : void
             *        {
             *            view = MyView.getInstance();
             *        }
             * </code>
             *
             * @return void
             */
            virtual void initializeController(void);

        public:
            /**
             * <code>Controller</code> Multiton Factory method.
             *
             * @return the Multiton instance of <code>Controller</code>
             */
            static IController &getInstance(std::string const &key = Controller::DEFAULT_KEY);

            /**
             * If an <code>ICommand</code> has previously been registered
             * to handle a the given <code>INotification</code>, then it is executed.
             *
             * @param notification an <code>INotification</code>
             */
            virtual void executeCommand(INotification const &notification);

            /**
             * Register a particular <code>ICommand</code> class as the handler
             * for a particular <code>INotification</code>.
             *
             * <P>
             * If an <code>ICommand</code> has already been registered to
             * handle <code>INotification</code>s with this name, it is no longer
             * used, the new <code>ICommand</code> is used instead.</P>
             *
             * The Observer for the new ICommand is only created if this the
             * first time an ICommand has been regisered for this Notification name.
             *
             * @param notification_name the name of the <code>INotification</code>
             * @param command the command of the <code>ICommand</code>
             */
            virtual void registerCommand(std::string const &notification_name, ICommand *command);

            /**
             * Check if a Command is registered for a given Notification
             *
             * @param notification_name
             * @return whether a Command is currently registered for the given <code>notification_name</code>.
             */
            virtual bool hasCommand(std::string const &notification_name) const;

            /**
             * Retrieve an <code>ICommand</code> instance from the Controller.
             *
             * @param notification_name the notification of the <code>INotification</code>
             * @return the <code>ICommand</code> instance previously registered with the given <code>notification</code>.
             */
            virtual ICommand const &retrieveCommand(std::string const &notification_name) const;

            /**
             * Remove a previously registered <code>ICommand</code> to <code>INotification</code> mapping.
             *
             * @param notification_name the name of the <code>INotification</code> to remove the <code>ICommand</code> mapping for
             * @return the <code>ICommand</code> that was removed from the <code>Controller</code>
             */
            virtual ICommand &retrieveCommand(std::string const &notification_name);

            /**
             * Remove a previously registered <code>ICommand</code> to <code>INotification</code> mapping.
             *
             * @param notification_name the name of the <code>INotification</code> to remove the <code>ICommand</code> mapping for
             * @return the <code>ICommand</code> that was removed from the <code>Controller</code>
             */
            virtual ICommand *removeCommand(std::string const &notification_name);

            /**
             * Clear all command and delete pointer
             */
            virtual void clearCommand();

            /**
             * List all notification name
             *
             * @return the aggregate container of <code>notification_name</code>.
             */
            virtual NotificationNames listNotificationNames(void) const;

            /**
             * Remove an IController instance
             *
             * @param key of IController instance to remove
             */
            static void removeController(std::string const &key = Controller::DEFAULT_KEY);

            /**
             * Virtual destructor.
             */
            virtual ~Controller(void);
        };
    }
}

#endif //_CONTROLLER_H_
