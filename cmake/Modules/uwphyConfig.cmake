INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_UWPHY uwphy)

FIND_PATH(
    UWPHY_INCLUDE_DIRS
    NAMES uwphy/api.h
    HINTS $ENV{UWPHY_DIR}/include
        ${PC_UWPHY_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    UWPHY_LIBRARIES
    NAMES gnuradio-uwphy
    HINTS $ENV{UWPHY_DIR}/lib
        ${PC_UWPHY_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/uwphyTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UWPHY DEFAULT_MSG UWPHY_LIBRARIES UWPHY_INCLUDE_DIRS)
MARK_AS_ADVANCED(UWPHY_LIBRARIES UWPHY_INCLUDE_DIRS)
