//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "command_interpreter/cmd.hpp"

#if !defined(HPX_DEBUG_MAIN_CMD_SEP_14_2013_0615PM)
#define HPX_DEBUG_MAIN_CMD_SEP_14_2013_0615PM

namespace hpx_debug
{
    class main_cmd : public command_interpreter::cmd
    {
    public:
        main_cmd();

        // Method called on an input line when the command prefix is not
        // recognized
        void default_command_handler(std::vector<std::string> const& args);

    private:
    };
}

#endif
