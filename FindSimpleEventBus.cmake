# - Try to find SimpleEventBus
# Once done this will define
#  SIMPLEEVENTBUS_FOUND - System has SimpleEventBus
#  SIMPLEEVENTBUS_INCLUDE_DIRS - The SimpleEventBus include directories
#  SIMPLEEVENTBUS_LIBRARIES - The libraries needed to use SimpleEventBus
#  SIMPLEEVENTBUS_DEFINITIONS - Compiler switches required for using SimpleEventBus

find_package(PkgConfig)
pkg_check_modules(PC_SIMPLEEVENTBUS QUIET libsimpleeventbus)
set(SIMPLEEVENTBUS_DEFINITIONS ${PC_SIMPLEEVENTBUS_CFLAGS_OTHER})

find_path(SIMPLEEVENTBUS_INCLUDE_DIR EventBus.h
          HINTS ${PC_SIMPLEEVENTBUS_INCLUDEDIR} ${PC_SIMPLEEVENTBUS_INCLUDE_DIRS}
          PATH_SUFFIXES simpleeventbus)

set(SIMPLEEVENTBUS_LIBRARIES "")
set(SIMPLEEVENTBUS_INCLUDE_DIRS ${SIMPLEEVENTBUS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SIMPLEEVENTBUS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SimpleEventBus  DEFAULT_MSG
                                  SIMPLEEVENTBUS_LIBRARY SIMPLEEVENTBUS_INCLUDE_DIR)

mark_as_advanced(SIMPLEEVENTBUS_INCLUDE_DIR SIMPLEEVENTBUS_LIBRARY)
