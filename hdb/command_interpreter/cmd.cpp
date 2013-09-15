//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "cmd.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <readline/readline.h>
#include <readline/history.h>

namespace command_interpreter
{
    ///////////////////////////////////////////////////////////////////////////
    cmd::cmd(std::string const& prompt, std::istream& istrm, std::ostream& ostrm)
      : prompt_(prompt), istrm_(istrm), ostrm_(ostrm)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    int cmd::loop(std::string const& intro)
    {
        ostrm_ << intro;

        // hook for pre-loop operations
        pre_loop();

        bool done = false;
        while (!done)
        {
            std::string input;
            {
                boost::shared_ptr<char> line(::readline(prompt_.c_str()), &::free);
                if (0 == line.get() || '\0' == line.get()[0])
                {
                    if (!emptyline() || !last_command_.empty())
                        continue;

                    // if not overridden, repeat last command
                    input = last_command_;
                }
                else 
                {
                    input = line.get();
                }
            }

            boost::algorithm::trim(input);
            
            // pre-command: optionally modify the input line
            input = pre_command(input);

            // stre this command line in the history
            ::add_history(input.c_str());

            // execute the command, returns whether to stop
            done = one_command(input);

            // post-command: optionally modify the done flag
            done = post_command(input, done);

            last_command_ = input;            
        }

        // hook for post-loop operations
        post_loop();

        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool cmd::add_command(std::string const& name, 
            boost::shared_ptr<command_base> command,
            std::string const& help)
    {
        command_infos_type::iterator it = commands_.find(name);
        if (it != commands_.end())
            return false;

        std::pair<command_infos_type::iterator, bool> p =
            commands_.insert(command_infos_type::value_type(
                name, command_info_type(command, boost::move(help)))
            );
        return p.second;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool cmd::one_command(std::string const& input)
    {
        return false;
    }
}