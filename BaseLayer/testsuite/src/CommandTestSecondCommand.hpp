#ifndef _COMMANDTESTSECONDCOMMAND_HPP_
#define _COMMANDTESTSECONDCOMMAND_HPP_

#include "CommandTestVO.hpp"

#include "../../Component/Patterns/Command/Command.h"
#include "../../Component/Interfaces/INotification.h"

namespace data
{
    using BaseLayer::Patterns::Command;
    using BaseLayer::Interfaces::INotification;
    /**
     * A Command subclass used by CommandTest.
     */
    struct CommandTestSecondCommand: Command
    {

        /**
         * Constructor.
         */
        CommandTestSecondCommand()
            :Command()
        { }
        
        /**
         * OUtput
         * 
         * @param event the <code>INotification</code> carrying the <code>CommandTestVO</code>
         */
        virtual void execute(INotification const& note)
        {
            CommandTestVO& vo= *(CommandTestVO*)note.getBody();
            // Fabricate a result
            vo.result += 3 * vo.input;
        }
    };
}

#endif //_COMMANDTESTSECONDCOMMAND_HPP_
