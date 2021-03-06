//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HDB_COMPONENT_SEP_16_2013_0158PM)
#define HDB_COMPONENT_SEP_16_2013_0158PM

#include <hpx/include/components.hpp>
#include <hdb_component/server/hdb_component.hpp>

namespace hpx_debug
{
    /// Client for the \a server::simple_accumulator component.
    class hdb_component
      : public hpx::components::client_base<
            hdb_component, server::hdb_component>
    {
        typedef hpx::components::client_base<
            hdb_component, server::hdb_component> base_type;

    public:
        /// Default construct an empty client side representation (not
        /// connected to any existing component).
        hdb_component() {}

        /// Create a client side representation for an existing
        /// \a server::hdb_component instance with the given GID.
        hdb_component(hpx::unique_future<hpx::naming::id_type> && gid)
          : base_type(gid.share())
        {}

        std::string get_locality_name() const
        {
            server::hdb_component::get_locality_name_action act;
            return act(get_gid());
        }
    };
}

#endif

