#ifndef _CONTROLLERTESTVO_HPP_
#define _CONTROLLERTESTVO_HPP_

namespace data
{
       /**
       * A utility class used by ControllerTest.
       * 
       * @see org.puremvc.as3.multicore.core.controller.ControllerTest ControllerTest
       * @see org.puremvc.as3.multicore.core.controller.ControllerTestCommand ControllerTestCommand
       */
    struct ControllerTestVO
    {
        int input;
        int result;
        /**
         * Constructor.
         * 
         * @param input the number to be fed to the ControllerTestCommand
         */
        ControllerTestVO(int input)
        {
            this->input = input;
            this->result = 0;
        }
    };
}

#endif //_CONTROLLERTESTVO_HPP_
