#ifndef _CONTROLLERTESTCOMMAND_HPP_
#define _CONTROLLERTESTCOMMAND_HPP_

#include "ControllerTestVO.hpp"
#include "../../Component/Patterns/Command/Command.h"

namespace data
{
    using BaseLayer::Patterns::Command;
    using BaseLayer::Interfaces::INotification;
    using BaseLayer::Interfaces::ICommand;
    /**
     * A SimpleCommand subclass used by ControllerTest.
     */
    struct ControllerTestCommand
        : public Command
    {
        /**
         * Constructor.
         */
        ControllerTestCommand(void)
            :Command()
        { }
        
        /**
         * Fabricate a result by multiplying the input by 2
         * 
         * @param note the note carrying the ControllerTestVO
         */
        virtual void execute(INotification const& note)
        {
            ControllerTestVO& vo= *(ControllerTestVO*)note.getBody();
            // Fabricate a result
            vo.result = 2 * vo.input;
        }
    };
}

#endif //_CONTROLLERTESTCOMMAND_HPP_
