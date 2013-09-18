//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hdb/hpx_cmd.hpp>
#include <hdb/commands/disconnect.hpp>
#include <hdb/commands/help.hpp>
#include <hdb/commands/license.hpp>
#include <hdb/commands/quit.hpp>
#include <hdb/commands/terminate.hpp>

#include <hdb_component/hdb_component.hpp>

#include <iostream>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace hpx_debug
{
    ///////////////////////////////////////////////////////////////////////////
    hpx_cmd::hpx_cmd(std::string const& appname)
      : istrm_device_(std::cin),
        istreamwrap_(istrm_device_, 1),         // set input buffer size to 1
        ostreamwrap_(boost::ref(std::cout)),
        cmd(appname, istreamwrap_, ostreamwrap_)
    {
        // add all known commands
        add_command(boost::make_shared<commands::disconnect>());
        add_command(boost::make_shared<commands::help>(boost::ref(*this)));
        add_command(boost::make_shared<commands::license>(boost::ref(*this)));
        add_command(boost::make_shared<commands::quit>());
        add_command(boost::make_shared<commands::terminate>());
    }

    ///////////////////////////////////////////////////////////////////////////
    void hpx_cmd::default_command_handler(std::vector<std::string> const& args)
    {
        cmd::default_command_handler(args);
        ostrm() << "Try 'help' command";
    }

    // Hook method executed once when loop() is called. This instantiates
    // the debugger component on locality#0 of the application and modifies the
    // prompt accordingly.
    void hpx_cmd::pre_loop()
    {
        // connect to locality#0
        std::string locality_name = connect_to_locality(0);
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string hpx_cmd::connect_to_locality(boost::uint32_t lid)
    {
        hpx::id_type id (hpx::naming::get_id_from_locality_id(lid));
        hdb_component c(hpx::new_<hdb_component::server_component_type>(id));

        return c.get_locality_name();
    }
}
