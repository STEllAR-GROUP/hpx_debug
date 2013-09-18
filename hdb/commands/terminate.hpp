//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_TERMINATE_CMD_SEP_18_2013_0822AM)
#define HPX_DEBUG_TERMINATE_CMD_SEP_18_2013_0822AM

#include <hdb/config.hpp>
#include <hdb/command_interpreter/command_base.hpp>

namespace hpx_debug { namespace commands
{
    class terminate : public command_interpreter::command_base
    {
    public:
        terminate() {}

        std::string get_command_name() const { return "terminate"; }

        bool do_call(std::vector<std::string> const& args);
        std::string do_help(command_interpreter::helpmode mode,
            std::vector<std::string> const& args) const;

    private:
    };
}}

#endif
