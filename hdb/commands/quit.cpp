//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/commands/quit.hpp>

namespace hpx_debug { namespace commands
{
    bool quit::do_call(std::vector<std::string> const& args)
    {
        command_interpreter::stop_cli_loop = true;
        return true;        // stop executing the command interpreter loop
    }

    std::string quit::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "exit the debugger";

        case command_interpreter::helpmode_command:
            return "quit\n"
                   "  -- exit the debugger";

        default:
            break;
        }
        return "";
    }
}}

