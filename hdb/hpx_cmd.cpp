//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_finalize.hpp>

#include "hpx_cmd.hpp"

#include "commands/help.hpp"
#include "commands/disconnect.hpp"
#include "commands/quit.hpp"

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace hpx_debug
{
    ///////////////////////////////////////////////////////////////////////////
    hpx_cmd::hpx_cmd(std::string const& appname)
      : cmd(appname, std::cin, std::cout)
    {
        // add all known commands
        add_command(boost::make_shared<commands::help>(boost::ref(*this)));
        add_command(boost::make_shared<commands::disconnect>());
        add_command(boost::make_shared<commands::quit>());
    }

    ///////////////////////////////////////////////////////////////////////////
    void hpx_cmd::default_command_handler(std::vector<std::string> const& args)
    {
        cmd::default_command_handler(args);
        ostrm() << "Try 'help' command";
    }
}