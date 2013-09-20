//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_finalize.hpp>

#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/commands/terminate.hpp>

namespace hpx_debug { namespace commands
{
    bool terminate::do_call(std::vector<std::string> const& args)
    {
        command_interpreter::stop_cli_loop = true;

        // non-gracefully terminate the whole application
        hpx::terminate();
        return true;
    }

    std::string terminate::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "terminate the debugged application, exit the debugger";

        case command_interpreter::helpmode_command:
            return "terminate\n"
                   "  -- terminate the debugged application";

        default:
            break;
        }
        return "";
    }
}}

