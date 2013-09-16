//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "help.hpp"

#include <algorithm>
#include <iomanip>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

namespace hpx_debug { namespace commands
{
    bool max_size(std::string const& lhs, std::string const& rhs)
    {
        return lhs.size() < rhs.size();
    }

    bool help::do_call(std::vector<std::string> const& args)
    {
        BOOST_ASSERT(!args.empty());

        std::vector<std::string> command_names(ci_.commands());
        if (command_names.empty())
        {
            throw std::runtime_error("no commands defined");
        }

        if (args.size() == 1 || args[1] == "all") {
            // print list of available commands
            std::vector<std::string>::iterator it = std::max_element(
                command_names.begin(), command_names.end(), max_size);
        
            BOOST_ASSERT(it != command_names.end());
            std::size_t max_length = (*it).size();
            
            ci_.ostrm() << "List of available commands:\n" << std::endl;

            BOOST_FOREACH(std::string const& name, command_names)
            {
                boost::shared_ptr<command_base> c = ci_.command(name);
                std::string help_string = c->do_help(
                    command_interpreter::helpmode_minimal, args);

                ci_.ostrm() << std::left << std::setw(max_length) 
                    << name << " -- " << help_string << std::endl;
            }

            ci_.ostrm() << "\nType \"help\" followed by a command name for "
                << "full documentation of that command." << std::endl;
        }
        else {
            // print help for given command
            if (!ci_.has_command(args[1]))
            {
                throw "can't provide help for unknown command: " + args[1];
            }

            boost::shared_ptr<command_base> c = ci_.command(args[1]);
            std::string help_string = c->do_help(
                command_interpreter::helpmode_command, args);

            ci_.ostrm() << help_string << std::endl;
        }
        return false;
    }

    std::string help::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "give short list of commands";
    
        case command_interpreter::helpmode_command:
            return "help <command> -- give short help for given command";

        default:
        case command_interpreter::helpmode_allcommands:
            break;
        }
        return "";
    }
}}

