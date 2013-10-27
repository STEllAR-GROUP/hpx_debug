# Copyright (c) 2007-2013 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# add hdb executable

set(dependencies "")
if(READLINE_FOUND)
  hpx_add_definitions(-DHDB_HAVE_READLINE)
  set(dependencies ${READLINE_LIBRARY})
endif()

add_hpx_executable(hdb
  AUTOGLOB
  HEADER_GLOB "*.h*"
  SOURCE_GLOB "*.c*"
  FOLDER "hdb/hdb"
  DEPENDENCIES ${dependencies}
  COMPONENT_DEPENDENCIES hdb_component
  ESSENTIAL)

