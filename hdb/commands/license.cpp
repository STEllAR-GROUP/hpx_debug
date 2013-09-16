//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <command_interpreter/cmd.hpp>
#include "license.hpp"

namespace hpx_debug { namespace commands
{
    char const* const copyright =
        "HDB - Interactive Debugger for HPX Applications\n\n"
        "Copyright (c) 2007-2013 The STE||AR Group, Louisiana State University,\n"
        "http://stellar.cct.lsu.edu, email:hpx-users@stellar.cct.lsu.edu\n\n"
        "Distributed under the Boost Software License, Version 1.0. (See accompanying\n"
        "file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)\n";

    bool license::do_call(std::vector<std::string> const& args)
    {
        ci_.ostrm() << copyright << std::endl;
        return false;
    }

    std::string license::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "print copyright and license for hdb";

        case command_interpreter::helpmode_command:
            return "license -- print copyright and license for hdb";

        default:
        case command_interpreter::helpmode_allcommands:
            break;
        }
        return "";
    }
}}

