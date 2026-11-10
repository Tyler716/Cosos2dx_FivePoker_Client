#ifndef _COMMANDTESTCOMMAND_HPP_
#define _COMMANDTESTCOMMAND_HPP_

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
    struct CommandTestCommand: Command
    {

        /**
         * Constructor.
         */
        CommandTestCommand()
            :Command()
        { }
        
        /**
         * Fabricate a result by multiplying the input by 2
         * 
         * @param event the <code>INotification</code> carrying the <code>CommandTestVO</code>
         */
        virtual void execute(INotification const& note)
        {
            CommandTestVO& vo= *(CommandTestVO*)note.getBody();
            // Fabricate a result
            vo.result = 2 * vo.input;
        }
    };
}

#endif //_COMMANDTESTCOMMAND_HPP_
