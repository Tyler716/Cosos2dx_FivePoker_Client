#include "../../Component/Patterns/Observer/Notification.h"

#include "../include/tut/tut.hpp"
#include "../include/tut_reporter.h"

namespace data
{
    struct NotificationTest
    { };
}

namespace testgroup
{
    typedef tut::test_group<data::NotificationTest> notification_test_t;
    typedef notification_test_t::object object;
#if 1
    notification_test_t notification_test("NotificationTest");
#endif
}

namespace tut
{
    using namespace testgroup;

    using BaseLayer::Interfaces::INotification;
    using BaseLayer::Patterns::Notification;

    template<> template<> 
    void object::test<1>(void)
    {
        set_test_name("testNameAccessors");

        INotification* note = new Notification("TestNote");

        // test assertions
        ensure_equals("Expecting note.getName() == 'TestNote'", note->getName(), "TestNote");

        delete note;
    }

    template<> template<> 
    void object::test<2>(void)
    {
        set_test_name("testBodyAccessors");

        int value = 5;
        INotification* note = new Notification("");

        note->setBody(&value);

        // test assertions
        ensure_equals("Expecting note.getBody()as Number == 5", *(int*)note->getBody(), 5);
        
        delete note;
    }

    template<> template<> 
    void object::test<3>(void)
    {
        set_test_name("testConstructor");

        int value = 5;
        INotification* note = new Notification("TestNote", &value, "TestNoteType");

        // test assertions
        ensure_equals( "Expecting note.getName() == 'TestNote'", note->getName(), "TestNote");
        ensure_equals( "Expecting note.getBody()as Number == 5",  *(int*)note->getBody(), 5);
        ensure_equals( "Expecting note.getType() == 'TestNoteType'", note->getType(), "TestNoteType");

        delete note;
    }

    template<> template<> 
    void object::test<4>(void)
    {
        set_test_name("testToString");

        int value[] = {1 ,3 ,5};
        INotification* note = new Notification("TestNote", value, "TestType");

        std::string ts = "Notification Name: TestNote\nBody: ";
        ts += typeid(void const*).name();
        ts += "\nType: TestType";

        std::string temp("Expecting note.testToString() == '");
        (temp += ts) += "'";
        std::string to_string = note->toString();
        //// test assertions
        ensure_equals(temp, to_string, ts );
            
        delete note;
    }
}
