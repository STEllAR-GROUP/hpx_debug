//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/commands/list.hpp>

namespace hpx_debug { namespace commands
{
    bool list::pre_call(std::vector<std::string> const& args)
    {
        return false;
    }

    bool list::do_call(std::vector<std::string> const& args)
    {
        return false;
    }

    std::string list::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "output vaious pieces of information about attached HPX application";

        case command_interpreter::helpmode_command:
            return "list -- output vaious pieces of information about attached HPX application";

        default:
        case command_interpreter::helpmode_allcommands:
            break;
        }
        return "";
    }
}}

