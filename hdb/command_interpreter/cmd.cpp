//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hdb/command_interpreter/cmd.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>

#if defined(HDB_HAVE_READLINE)
#  include <readline/readline.h>
#  include <readline/history.h>

#  define HDB_COMMAND_HISTORY_LEN 100
#endif

namespace command_interpreter
{
    ///////////////////////////////////////////////////////////////////////////
    bool stop_cli_loop = false;

    ///////////////////////////////////////////////////////////////////////////
    void cmd::init_history()
    {
#if defined(HDB_HAVE_READLINE)
        char const* env = std::getenv("HOME");
        if (!env) return;

        history_ = std::string(env) + "/." + appname_ + ".rc";

        ::using_history();
        ::read_history(history_.c_str());
#endif
    }


    void cmd::close_history()
    {
#if defined(HDB_HAVE_READLINE)
        if (history_.empty()) return;

        ::write_history(history_.c_str());
        ::history_truncate_file(history_.c_str(), HDB_COMMAND_HISTORY_LEN);
#endif
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
        prompt_ = prompt;

        if (!intro.empty())
            ostrm_ << intro;

        // hook for pre-loop operations
        pre_loop();

        done_ = false;
        while (!done())
        {
            std::string input;
            {
#if defined(HDB_HAVE_READLINE)
                boost::shared_ptr<char> line(::readline(prompt_.c_str()), &::free);
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
#else
                ostrm_ << prompt << std::flush;
                if (!std::getline(istrm_, input))
                {
                    break;
                }
                else if (input.empty())
                {
                    if (!emptyline() && last_command_.empty())
                        continue;

                    // if not overridden, repeat last command
                    input = last_command_;
                }
#endif
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
    bool cmd::pre_command(std::vector<std::string>& args)
    {
        // find command based on (possibly abbreviated) input
        return resolve_approximate_command(args[0]);
    }

    bool cmd:: post_command(std::vector<std::string> const& args, bool stop)
    {
        return stop;
    }

    bool cmd::one_command(std::string const& inp)
    {
        std::vector<std::string> args;

        try {
            boost::algorithm::split(args, inp,
                boost::algorithm::is_any_of(" \t"),
                boost::algorithm::token_compress_on);

            // pre-command: optionally modify the input arguments, possibly
            // expand abbreviated commands and arguments, return whether
            // modifications have been made
            bool cmd_is_approx = pre_command(args);

            // remove the command from arguments array
            std::string resolved_command = args[0];
            args.erase(args.begin());

            // do default handling if the given command is not recognized
            if (!has_command(resolved_command))
            {
                default_command_handler(args);
                return post_command(args, false);
            }

            // pre-command: optionally modify the input arguments, return
            // whether modifications have been made
            boost::shared_ptr<command_base> c = command(resolved_command);

            // pre-call: optionally modify the input arguments, possibly
            // expand abbreviated commands and arguments, return whether
            // modifications have been made
            cmd_is_approx |= c->pre_call(args, 0);

            // print full command if user specified something abbreviated
            std::string input(inp);
            if (cmd_is_approx)
            {
                input = resolved_command;
                BOOST_FOREACH(std::string const& str, args)
                {
                    input += " " + str;
                }
                ostrm_ << input << std::endl;
            }

            // execute command
            bool result = c->do_call(args);

#if defined(HDB_HAVE_READLINE)
            // store this command line in the history
            if (input != last_command_)
                ::add_history(input.c_str());
#endif

            return post_command(args, result);
        }
        catch (std::runtime_error const& e) {
            ostrm_ << "caught exception: " << e.what() << std::endl;
        }
        catch (...) {
            ostrm_ << "caught unexpected exception" << std::endl;
        }

        return post_command(args, false);
    }

    void cmd::default_command_handler(std::vector<std::string> const& args)
    {
        ostrm_ << "unknown command name: " << args[0] << "." << std::endl;
    }

    void cmd::pre_loop()
    {
        init_history();
    }

    void cmd::post_loop()
    {
        close_history();
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

    // find a matching command if it's unambiguous
    cmd::command_infos_type::const_iterator cmd::find_command(
        std::string const& name) const
    {
        std::size_t cnt_matching = 0;

        command_infos_type::const_iterator end = commands_.end();
        command_infos_type::const_iterator entry_id = end;

        for (command_infos_type::const_iterator it = commands_.lower_bound(name);
             it != end; ++it)
        {
            if ((*it).first.find(name) == 0)
            {
                if (cnt_matching == 0)
                    entry_id = it;
                ++cnt_matching;
            }
        }

        return cnt_matching == 1 ? entry_id : end;
    }

    boost::shared_ptr<command_base> cmd::command(std::string const& name) const
    {
        command_infos_type::const_iterator it = find_command(name);
        if (it == commands_.end())
        {
            throw std::runtime_error("unknown or ambigous command name: " + name);
        }
        return (*it).second;
    }

    bool cmd::resolve_approximate_command(std::string& name) const
    {
        command_infos_type::const_iterator it = find_command(name);
        if (it != commands_.end() && (*it).first != name)
        {
            name = (*it).first;
            return true;
        }
        return false;
    }

    bool cmd::has_command(std::string const& name) const
    {
        command_infos_type::const_iterator it = commands_.find(name);
        return (it != commands_.end()) ? true : false;
    }
}
