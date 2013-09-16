//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "main_cmd.hpp"

#include "commands/help.hpp"
#include "commands/quit.hpp"
#include "commands/connect.hpp"

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <readline/readline.h>

#if !defined(BOOST_WINDOWS)
#  include <signal.h>
#  include <stdlib.h>
#  include <string.h>
#endif

///////////////////////////////////////////////////////////////////////////////
namespace hpx_debug
{
    void handle_termination(int signum)
    {
        command_interpreter::stop_cli_loop = true;
    }

#if defined(BOOST_WINDOWS)
    BOOL WINAPI termination_handler(DWORD ctrl_type)
    {
        switch (ctrl_type) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            handle_termination(ctrl_type);
            return TRUE;

        default:
            break;
        }
        return FALSE;
    }
#endif

    void set_error_handlers()
    {
#if defined(BOOST_WINDOWS)
        // Set console control handler to allow server to be stopped.
        SetConsoleCtrlHandler(termination_handler, TRUE);
#else
        struct sigaction new_action;
        new_action.sa_handler = handle_termination;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;

        sigaction(SIGINT, &new_action, NULL);  // Interrupted
        sigaction(SIGBUS, &new_action, NULL);  // Bus error
        sigaction(SIGFPE, &new_action, NULL);  // Floating point exception
        sigaction(SIGILL, &new_action, NULL);  // Illegal instruction
        sigaction(SIGPIPE, &new_action, NULL); // Bad pipe
        sigaction(SIGSEGV, &new_action, NULL); // Segmentation fault
        sigaction(SIGSYS, &new_action, NULL);  // Bad syscall
#endif
    }

    ///////////////////////////////////////////////////////////////////////////
    char const* const hdb_name = "hdb";

    ///////////////////////////////////////////////////////////////////////////
    main_cmd::main_cmd()
      : cmd(hdb_name, std::cin, std::cout)
    {
        // initialize signal handlers
        set_error_handlers();

        // Allow conditional parsing of the ~/.inputrc file.
        rl_readline_name = hdb_name;

        // add all known commands
        add_command(boost::make_shared<commands::help>(boost::ref(*this)));
        add_command(boost::make_shared<commands::quit>());
        add_command(boost::make_shared<commands::connect>(boost::ref(*this)));
    }

    ///////////////////////////////////////////////////////////////////////////
    void main_cmd::default_command_handler(std::vector<std::string> const& args)
    {
        cmd::default_command_handler(args);
        ostrm() << "Try \"help\" command";
    }
}