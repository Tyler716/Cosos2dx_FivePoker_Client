#ifndef _COMMANDTESTVO_HPP_
#define _COMMANDTESTVO_HPP_

namespace data
{
    /**
     * A utility class used by CommandTest.
     */
    struct CommandTestVO
    {
        int input;
        int result;
        /**
         * Constructor.
         * 
         * @param input the number to be fed to the CommandTestCommand
         */
        CommandTestVO(int input)
        {
            this->input = input;
            this->result = 0;
        }
    };
}

#endif //_COMMANDTESTVO_HPP_
