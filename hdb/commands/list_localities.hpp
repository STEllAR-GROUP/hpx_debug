//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_LIST_LOCALITIES_CMD_SEP_19_2013_0549PM)
#define HPX_DEBUG_LIST_LOCALITIES_CMD_SEP_19_2013_0549PM

#include <hdb/config.hpp>
#include <hdb/command_interpreter/command_base.hpp>

namespace hpx_debug { namespace commands
{
    class list_localities : public command_interpreter::command_base
    {
    public:
        list_localities() {}

        std::string get_command_name() const { return "list_localities"; }

        bool do_call(std::vector<std::string> const& args);
        std::string do_help(command_interpreter::helpmode mode,
            std::vector<std::string> const& args) const;
    };
}}

#endif
