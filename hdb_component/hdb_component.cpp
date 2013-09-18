//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/serialization.hpp>

#include <hdb_component/server/hdb_component.hpp>

///////////////////////////////////////////////////////////////////////////////
// Add factory registration functionality.
HPX_REGISTER_COMPONENT_MODULE();

///////////////////////////////////////////////////////////////////////////////
typedef hpx::components::simple_component<
    hpx_debug::server::hdb_component
> hdb_component_type;

HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(hdb_component_type, hdb_component);

///////////////////////////////////////////////////////////////////////////////
// Serialization support for simple_accumulator actions.
HPX_REGISTER_ACTION(
    hdb_component_type::wrapped_type::get_locality_name_action,
    hdb_component_get_locality_name_action);

HPX_REGISTER_ACTION(
    hpx::components::server::create_component_action0<hpx_debug::server::hdb_component>,
    hdb_component_create_component_action);

