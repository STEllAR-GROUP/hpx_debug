//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_QUIT_CMD_SEP_14_2013_0637PM)
#define HPX_DEBUG_QUIT_CMD_SEP_14_2013_0637PM

#include <config.hpp>
#include <command_interpreter/command_base.hpp>

namespace hpx_debug { namespace commands
{
    class quit : public command_interpreter::command_base
    {
    public:
        quit() {}

        std::string get_command_name() const { return "quit"; }

        bool do_call(std::vector<std::string> const& args);
        std::string do_help(command_interpreter::helpmode mode,
            std::vector<std::string> const& args) const;

    private:
    };
}}

#endif
