//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hdb/commands/disconnect.hpp>

namespace hpx_debug { namespace commands
{
    bool disconnect::do_call(std::vector<std::string> const& args)
    {
        // hpx::disconnect() is called in the connect command implementation
        return true;
    }

    std::string disconnect::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "disconnect from the HPX application";

        case command_interpreter::helpmode_command:
            return "disconnect -- disconnect from the HPX application";

        default:
        case command_interpreter::helpmode_allcommands:
            break;
        }
        return "";
    }
}}

