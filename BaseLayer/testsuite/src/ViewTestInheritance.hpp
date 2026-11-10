#ifndef _VIEWTESTHERITANCE_HPP_
#define _VIEWTESTHERITANCE_HPP_

#include "../../Core/View/View.h"

namespace data
{
    using BaseLayer::Interfaces::IView;
    using BaseLayer::Core::View;

    class ViewTestInheritance 
        : public View
    {
        friend class View;
    public:
        std::string* initializeViewCalled;
    protected:
        ViewTestInheritance(void)
        :View(this, "ViewTestInheritance")
        { }
    public:
        static ViewTestInheritance& getInstance(void)
        {
            if (View::find("ViewTestInheritance") == NULL)
            {
                new ViewTestInheritance();
            }

            return *(dynamic_cast<ViewTestInheritance*>(&View::getInstance("ViewTestInheritance")));
        }
    protected:
        void initializeView(void)
        {
            initializeViewCalled = new std::string("Called!!!");
            View::initializeView();
        }
    public:
        ~ViewTestInheritance()
        {
            delete initializeViewCalled;
        }
    };
}

#endif //_VIEWTESTHERITANCE_HPP_
