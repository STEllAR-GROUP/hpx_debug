//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_LICENSE_CMD_SEP_15_2013_0844PM)
#define HPX_DEBUG_LICENSE_CMD_SEP_15_2013_0844PM

#include <config.hpp>
#include <command_interpreter/cmd.hpp>
#include <command_interpreter/command_base.hpp>

namespace hpx_debug { namespace commands
{
    class license : public command_interpreter::command_base
    {
    public:
        license(command_interpreter::cmd& ci) : ci_(ci) {}

        std::string get_command_name() const { return "license"; }

        bool do_call(std::vector<std::string> const& args);
        std::string do_help(command_interpreter::helpmode mode,
            std::vector<std::string> const& args) const;

    private:
        command_interpreter::cmd& ci_;
    };
}}

#endif
