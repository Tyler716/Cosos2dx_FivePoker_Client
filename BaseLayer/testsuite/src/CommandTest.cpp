#include "CommandTestCommand.hpp"
#include "CommandTestVO.hpp"
#include "CommandTestFirstCommand.hpp"
#include "CommandTestSecondCommand.hpp"

#include "../../Component/Patterns/Facade/Facade.h"
#include "../../Component/Patterns/Observer/Notification.h"

#include "../include/tut/tut.hpp"
#include "../include/tut_reporter.h"

namespace data
{
    struct CommandTest
    { };
}

namespace testgroup
{
    typedef tut::test_group<data::CommandTest> command_test_t;
    typedef command_test_t::object object;
#if 1
    command_test_t command_test("CommandTest");
#endif
}

namespace tut
{
    using namespace testgroup;

    using BaseLayer::Interfaces::INotification;
    using BaseLayer::Interfaces::IFacade;
    using BaseLayer::Patterns::Notification;
    using BaseLayer::Patterns::Facade;
#if 1
    template<> template<> 
    void command_test_t::object::test<1>(void)
    {
        set_test_name("testCommandExecute");

        data::CommandTestVO vo(5);
        Notification note("CommandTestNote", &vo);
        data::CommandTestCommand command;

        command.execute((INotification&)note);

        ensure_equals( "Expecting vo.result == 10", vo.result, 10 );
    }
#endif
#if 1
    template<> template<> 
    void command_test_t::object::test<2>(void)
    {
        set_test_name("testRegisterCommandInCommand");

        // create dummy facade.
        IFacade &facade = Facade::getInstance();

        // create test value object
        // vo.input = 10;
        // v.result = 0;
        data::CommandTestVO vo(10);

        // Register  test command first.
        data::CommandTestFirstCommand command;
        facade.registerCommand("CommandTestFirstCommand", &command);

        // activating command first.
        facade.sendNotification("CommandTestFirstCommand", &vo);

        // The result should be 50
        // vo.result += 2 * vo.input;
        // vo.result += 3 * vo.input;
        ensure_equals( "Expecting vo.result == 50", vo.result, 50 );

        // activating again command second.
        facade.sendNotification("CommandTestSecondCommand", &vo);

        // result should be 80
        // vo.result += 3 * vo.input;
        ensure_equals( "Expecting vo.result == 80", vo.result, 80 );

        // activating command first.
        facade.sendNotification("CommandTestFirstCommand", &vo);

        // The result does not change
        ensure_equals( "Expecting vo.result == 80", vo.result, 80 );
    }
#endif
}
