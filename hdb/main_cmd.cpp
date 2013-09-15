//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include "main_cmd.hpp"

#include "commands/help.hpp"
#include "commands/quit.hpp"

#include <iostream>

namespace hpx_debug
{
    ///////////////////////////////////////////////////////////////////////////
    char const* const hdb_prompt = "hdb> ";

    ///////////////////////////////////////////////////////////////////////////
    main_cmd::main_cmd()
      : cmd(hdb_prompt, std::cin, std::cout)
    {
        add_command("help", boost::make_shared<commands::help>(boost::ref(*this)));
        add_command("quit", boost::make_shared<commands::quit>());
    }

    ///////////////////////////////////////////////////////////////////////////
    void main_cmd::default_command_handler(std::vector<std::string> const& args)
    {
        cmd::default_command_handler(args);
        ostrm() << "Try 'help' command";
    }
}