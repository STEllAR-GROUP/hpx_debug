//  Copyright (c) 2007-2011 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Adelstein-Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HDB_SERVER_COMPONENT_SEP_16_2013_0201PM)
#define HDB_SERVER_COMPONENT_SEP_16_2013_0201PM

#include <hpx/hpx_fwd.hpp>
#include <hpx/include/components.hpp>

#include <string>

#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hpx_debug { namespace server
{
    // 
    class hdb_component
      : public hpx::components::simple_component_base<hdb_component>
    {
    public:
        hdb_component() {}

        std::string get_locality_name() const
        {
            return "locality#" +
                boost::lexical_cast<std::string>(hpx::get_locality_id());
        }

        HPX_DEFINE_COMPONENT_CONST_ACTION(hdb_component, get_locality_name);
    };
}}

HPX_REGISTER_ACTION_DECLARATION(
    hpx_debug::server::hdb_component::get_locality_name_action,
    hdb_component_get_locality_name_action);

HPX_REGISTER_ACTION_DECLARATION(
    hpx::components::server::create_component_action0<hpx_debug::server::hdb_component>,
    hdb_component_create_component_action);

#endif

