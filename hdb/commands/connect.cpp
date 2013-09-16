//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_start.hpp>

#include "connect.hpp"

namespace hpx_debug { namespace commands
{
    bool connect::do_call(std::vector<std::string> const& args)
    {
        std::vector<std::string> cfg;

        hpx::start(cfg, hpx::runtime_mode_connect);

        return false;
    }

    std::string connect::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "connect to a running HPX application";

        case command_interpreter::helpmode_command:
            return "connect <locality> -- connect to an HPX application running on <locality>";

        default:
        case command_interpreter::helpmode_allcommands:
            break;
        }
        return "";
    }
}}

