//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hdb/config.hpp>
#include <hdb/command_interpreter/cmd.hpp>
#include <hdb/util/wrap_istream.hpp>
#include <hdb/util/wrap_ostream.hpp>

#include <boost/iostreams/stream.hpp>

#if !defined(HPX_DEBUG_HPX_LIST_CMD_SEP_19_2013_0522PM)
#define HPX_DEBUG_HPX_LIST_CMD_SEP_19_2013_0522PM

namespace hpx_debug
{
    class hpx_list_cmd : public command_interpreter::cmd
    {
    public:
        hpx_list_cmd(std::string const& appname, std::istream& istrm,
            std::ostream& ostrm);
    };
}

#endif
