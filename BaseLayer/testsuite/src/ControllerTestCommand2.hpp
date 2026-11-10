#ifndef _CONTROLLERTESTCOMMAND2_HPP_
#define _CONTROLLERTESTCOMMAND2_HPP_

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
    struct ControllerTestCommand2
        : public Command
    {
        /**
         * Constructor.
         */
        ControllerTestCommand2(void)
            :Command()
        { }

        /**
         * Fabricate a result by multiplying the input by 2 and adding to the existing result
         * <P>
         * This tests accumulation effect that would show if the command were executed more than once.
         * @param note the note carrying the ControllerTestVO
         */
        virtual void execute(INotification const& note)
        {
            ControllerTestVO& vo= *(ControllerTestVO*)note.getBody();
            // Fabricate a result
            vo.result = vo.result+(2 * vo.input);
        }
    };
}

#endif /* __CONTROLLER_TEST_COMMAND2_HPP__ */
