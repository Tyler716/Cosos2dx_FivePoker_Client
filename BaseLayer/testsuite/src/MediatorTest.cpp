#include "../../Component/Patterns/Mediator/Mediator.h"

#include "../include/tut/tut.hpp"
#include "../include/tut_reporter.h"


namespace data
{
    struct MediatorTest
    { };
}

namespace testgroup
{
    typedef tut::test_group<data::MediatorTest> mediator_test_t;
    typedef mediator_test_t::object object ;
#if 1
    mediator_test_t mediator_test("MediatorTest");
#endif
}

namespace tut
{
    using namespace testgroup;
    
    using BaseLayer::Patterns::Mediator;

    template<> template<> 
    void object::test<1>(void)
    {
        set_test_name("testNameAccessor");

        Mediator mediator;
        ensure_equals("Expecting mediator.getMediatorName() == Mediator.NAME", mediator.getMediatorName(), Mediator::NAME);
    }

    template<> template<> 
    void object::test<2>(void)
    {
        set_test_name("testViewAccessor");

        std::string *str = new std::string();
        Mediator mediator(Mediator::NAME, str);

        ensure( "Expecting mediator.getViewComponent() not null", mediator.getViewComponent() != NULL );

        delete str;
    }
}
