//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hdb/hpx_list_cmd.hpp>
#include <hdb/commands/list_localities.hpp>

namespace hpx_debug
{
    ///////////////////////////////////////////////////////////////////////////
    hpx_list_cmd::hpx_list_cmd(std::string const& appname, std::istream& istrm,
            std::ostream& ostrm)
      : cmd(appname, istrm, ostrm)
    {
        // add all known commands
        add_command(boost::make_shared<commands::list_localities>());
    }
}
