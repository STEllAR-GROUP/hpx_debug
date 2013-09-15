//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_COMMAND_BASE_SEP_14_2013_0748PM)
#define HPX_DEBUG_COMMAND_BASE_SEP_14_2013_0748PM

#include <iosfwd>
#include <string>
#include <vector>

namespace command_interpreter
{
    enum helpmode
    {
        helpmode_minimal = 0,
        helpmode_full = 1
    };

    struct command_base
    {
        virtual ~command_base() {}

        virtual bool do_call(std::vector<std::string> const& args) = 0;
        virtual std::string do_help(helpmode mode) const = 0;
    };
}

#endif
