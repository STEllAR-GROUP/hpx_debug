//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/commands/list.hpp>

namespace hpx_debug { namespace commands
{
    list::list(std::string const& appname, std::istream& istrm,
            std::ostream& ostrm)
        : ci_(appname, istrm, ostrm)
    {}

    bool list::pre_call(std::vector<std::string>& args, std::size_t index)
    {
        if (args.empty())
            return false;

        if (!ci_.resolve_approximate_command(args[0]))
            return false;

        if (!ci_.has_command(args[0]))
            return false;       // we report the error later

        std::string resolved_command(args[0]);
        boost::shared_ptr<command_base> c = ci_.command(resolved_command);

        c->pre_call(args, index+1);

        return true;
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
            return "output various pieces of information about attached HPX application";

        case command_interpreter::helpmode_command:
            return "list [<item>]\n"
                   "  -- output various pieces of information about attached HPX application";

        default:
            break;
        }
        return "";
    }
}}

