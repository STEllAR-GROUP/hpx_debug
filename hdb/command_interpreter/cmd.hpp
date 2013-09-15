//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <command_interpreter/command_base.hpp>

#include <iosfwd>
#include <string>
#include <vector>
#include <map>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/move.hpp>

#if !defined(HPX_DEBUG_CMD_SEP_14_2013_0614PM)
#define HPX_DEBUG_CMD_SEP_14_2013_0614PM

namespace command_interpreter
{
    class cmd
    {
    private:
        ///////////////////////////////////////////////////////////////////////
        typedef std::map<std::string, boost::shared_ptr<command_base> >
            command_infos_type;

    public:
        ///////////////////////////////////////////////////////////////////////
        cmd(std::string const& prompt, std::istream& istrm, 
            std::ostream& ostrm);

        virtual ~cmd();

        ///////////////////////////////////////////////////////////////////////
        // do command line processing
        int loop(std::string const& intro);

        // add a command to the interpreter
        bool add_command(std::string const& name, 
            boost::shared_ptr<command_base> command);

        ///////////////////////////////////////////////////////////////////////
        virtual void pre_loop() {}
        virtual void post_loop();

        // called when imput line was empty
        virtual bool emptyline()
        {
            return false;
        }

        // Hook method executed just before the command line 'input' is
        // interpreted
        virtual std::string pre_command(std::string const& input)
        {
            return input;
        }

        // Hook method executed just after a command dispatch is finished
        virtual bool post_command(std::string const& input, bool stop)
        {
            return stop;
        }

        // Interpret the argument as though it had been typed in response
        // to the prompt
        virtual bool one_command(std::string const& input);

        // Method called on an input line when the command prefix is not
        // recognized
        virtual void default_command_handler(std::vector<std::string> const& args);

        ///////////////////////////////////////////////////////////////////////
        std::vector<std::string> commands() const;
        boost::shared_ptr<command_base> command(std::string const& name) const;
        bool has_command(std::string const& name) const;

        std::istream& istrm() { return istrm_; }
        std::ostream& ostrm() { return ostrm_; }

    private:
        std::string prompt_;
        std::istream& istrm_;
        std::ostream& ostrm_;

        command_infos_type commands_;

        std::string last_command_;      // last successfully executed command
    };
}

#endif
