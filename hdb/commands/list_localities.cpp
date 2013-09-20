//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/commands/list_localities.hpp>

namespace hpx_debug { namespace commands
{
    bool list_localities::do_call(std::vector<std::string> const& args)
    {
        return false;
    }

    std::string list_localities::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "output information about given localities";

        case command_interpreter::helpmode_command:
            return "list localities [<which>]\n"
                   "  -- output information about given localities\n"
                   "     where <which> specifies the target localities to list\n"
                   "                   the information for\n"
                   "                   (default: currenty active locality)";

        default:
            break;
        }
        return "";
    }
}}

