#ifndef _MODELTESTINHERITANCE_HPP_
#define _MODELTESTINHERITANCE_HPP_

#include "../../Component/Interfaces/IModel.h"
#include "../../Core/Model/Model.h"

// STL include
#include <string>
#include <iostream>

namespace data
{
    using BaseLayer::Interfaces::IModel;
    using BaseLayer::Core::Model;

    class ModelTestInheritance 
        : public Model
    {
        friend class Model;

    public:
        std::string *initializeModelCalled;

    protected:
        ModelTestInheritance(void) : Model(this, "ModelTestInheritance")
        {
        }

    public:
        static ModelTestInheritance &getInstance(void)
        {
            if (Model::find("ModelTestInheritance") == NULL)
            {
                new ModelTestInheritance();
            }

            return *(dynamic_cast<ModelTestInheritance*>(&Model::getInstance("ModelTestInheritance")));
        }

    protected:
        void initializeModel(void)
        {
            initializeModelCalled = new std::string("Called!!!");
            Model::initializeModel();
        }
    public:
        ~ModelTestInheritance()
        {
            delete initializeModelCalled;
        }
    };
}

#endif //_MODELTESTINHERITANCE_HPP_
