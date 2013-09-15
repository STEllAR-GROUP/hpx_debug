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

    cmd::~cmd()
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
                    if (rl_done)
                        done = true;

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
            boost::shared_ptr<command_base> command)
    {
        command_infos_type::iterator it = commands_.find(name);
        if (it != commands_.end())
            return false;

        std::pair<command_infos_type::iterator, bool> p =
            commands_.insert(command_infos_type::value_type(
                name, command));
        return p.second;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool cmd::one_command(std::string const& input)
    {
        try {
            std::vector<std::string> args(1, input);

            if (!has_command(args[0])) {
                default_command_handler(args);
            }
            else {
                boost::shared_ptr<command_base> c = command(input);

                return c->do_call(args);
            }
        }
        catch (std::runtime_error const& e) {
            ostrm_ << "caught exception: " << e.what();
        }
        catch (...) {
            ostrm_ << "caught unexpected exception";
        }
        ostrm_ << std::endl;
        return false;
    }

    void cmd::default_command_handler(std::vector<std::string> const& args)
    {
        ostrm_ << "unknown command name: " << args[0];
    }

    void cmd::post_loop()
    {
        ostrm_ << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    std::vector<std::string> cmd::commands() const
    {
        std::vector<std::string> command_names;
        command_names.reserve(commands_.size());

        command_infos_type::const_iterator end = commands_.end();
        for (command_infos_type::const_iterator it = commands_.begin(); 
             it != end; ++it)
        {
            command_names.push_back((*it).first);
        }

        return command_names;
    }

    boost::shared_ptr<command_base> cmd::command(std::string const& name) const
    {
        command_infos_type::const_iterator it = commands_.find(name);
        if (it == commands_.end())
        {
            throw std::runtime_error("unknown command name: " + name);
        }
        return (*it).second;
    }

    bool cmd::has_command(std::string const& name) const
    {
        command_infos_type::const_iterator it = commands_.find(name);
        return (it != commands_.end()) ? true : false;
    }
}