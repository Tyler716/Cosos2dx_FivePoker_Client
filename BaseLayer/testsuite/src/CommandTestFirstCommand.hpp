#ifndef _COMMANDTESTFIRSTCOMMAND_HPP_
#define _COMMANDTESTFIRSTCOMMAND_HPP_

#include "CommandTestVO.hpp"
#include "CommandTestSecondCommand.hpp"

#include "../../Component/Patterns/Command/Command.h"
#include "../../Component/Interfaces/INotification.h"

namespace data
{
    using BaseLayer::Patterns::Command;
    using BaseLayer::Interfaces::INotification;
    /**
     * A CommandTestFirstCommand subclass used by CommandTest.
     */
    struct CommandTestFirstCommand: Command
    {

        /**
         * Constructor.
         */
        CommandTestFirstCommand()
            :Command()
        { }
        
        /**
         * It just register CommandTestSecondCommand
         * 
         * @param event the <code>INotification</code> carrying the <code>CommandTestVO</code>
         */
        virtual void execute(INotification const& note)
        {
            // Combine test. Remove itself.
            getFacade().removeCommand("CommandTestFirstCommand");

            // Send itself again.
            sendNotification("CommandTestFirstCommand", note.getBody(), note.getType());

            CommandTestVO& vo= *(CommandTestVO*)note.getBody();
            // Fabricate a result
            vo.result += 2 * vo.input;

            // Register second command.
            static CommandTestSecondCommand command;
            getFacade().registerCommand("CommandTestSecondCommand", &command);

            // Then sending notification.
            sendNotification("CommandTestSecondCommand", note.getBody(), note.getType());
        }
    };
}

#endif //_COMMANDTESTFIRSTCOMMAND_HPP_
