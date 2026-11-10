#ifndef _FACADETESTCOMMAND_HPP_
#define _FACADETESTCOMMAND_HPP_

#include "FacadeTestVO.hpp"
#include "../../Component/Patterns/Command/Command.h"
#include "../../Component/Interfaces/INotification.h"

namespace data
{
    using BaseLayer::Patterns::Command;
    using BaseLayer::Interfaces::INotification;

    /**
     * A SimpleCommand subclass used by FacadeTest.
     *
     * @see org.puremvc.as3.multicore.patterns.facade.FacadeTest FacadeTest
     * @see org.puremvc.as3.multicore.patterns.facade.FacadeTestVO FacadeTestVO
     */
    struct FacadeTestCommand : public Command
    {
        void execute(INotification const& notification)
        {
            FacadeTestVO& vo =  *(FacadeTestVO*) notification.getBody();
            vo.result = vo.input * 2;
        }
    };
}

#endif //_FACADETESTCOMMAND_HPP_
