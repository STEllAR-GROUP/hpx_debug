//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "cmd.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>

#include <readline/readline.h>
#include <readline/history.h>

#define CMD_HISTORY_LEN 100

namespace command_interpreter
{
    ///////////////////////////////////////////////////////////////////////////
    bool stop_cli_loop = false;

    ///////////////////////////////////////////////////////////////////////////
    void cmd::init_history()
    {
        char const* env = std::getenv("HOME");
        if (env)
        {
            history_ = std::string(env) + "/." + appname_ + ".rc";

            ::using_history();
            ::read_history(history_.c_str());
        }
    }


    void cmd::close_history()
    {
        if (!history_.empty())
        {
            ::write_history(history_.c_str());
            ::history_truncate_file(history_.c_str(), CMD_HISTORY_LEN);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    cmd::cmd(std::string const& appname, std::istream& istrm, std::ostream& ostrm)
      : appname_(appname), istrm_(istrm), ostrm_(ostrm), done_(false)
    {
        stop_cli_loop = false;
    }

    cmd::~cmd()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    int cmd::loop(std::string const& prompt, std::string const& intro)
    {
        if (!intro.empty())
            ostrm_ << intro;

        // hook for pre-loop operations
        pre_loop();

        done_ = false;
        while (!done())
        {
            std::string input;
            {
                boost::shared_ptr<char> line(::readline(prompt.c_str()), &::free);
                if (0 == line.get())
                {
                    break;
                }
                else if ('\0' == line.get()[0])
                {
                    if (!emptyline() && last_command_.empty())
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
            
            // execute the command, returns whether to stop
            done_ = one_command(input);

            last_command_ = input;            
        }

        // hook for post-loop operations
        post_loop();

        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool cmd::add_command(boost::shared_ptr<command_base> command)
    {
        std::string name = command->get_command_name();

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
        // pre-command: optionally modify the input line
        std::string inp(pre_command(input));

        try {
            std::vector<std::string> args;
            boost::algorithm::split(args, inp, 
                boost::algorithm::is_any_of(" \t"), 
                boost::algorithm::token_compress_on); 

            if (!has_command(args[0])) {
                default_command_handler(args);
                return post_command(inp, false);
            }

            // execute command
            boost::shared_ptr<command_base> c = command(args[0]);
            bool result = c->do_call(args);

            // store this command line in the history
            if (inp != last_command_)
                ::add_history(inp.c_str());

            return post_command(inp, result);
        }
        catch (std::runtime_error const& e) {
            ostrm_ << "caught exception: " << e.what();
        }
        catch (...) {
            ostrm_ << "caught unexpected exception";
        }

        ostrm_ << std::endl;
        return post_command(inp, false);
    }

    void cmd::default_command_handler(std::vector<std::string> const& args)
    {
        ostrm_ << "unknown command name: " << args[0] << ". ";
    }

    void cmd::pre_loop()
    {
        init_history();
    }

    void cmd::post_loop()
    {
        close_history();
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