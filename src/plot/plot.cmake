#========================================================================================

find_package( OpenCV REQUIRED )

#========================================================================================

include_directories( ${CMAKE_CURRENT_LIST_DIR} )

#========================================================================================

set( PLOT
    ${CMAKE_CURRENT_LIST_DIR}/view.h
    ${CMAKE_CURRENT_LIST_DIR}/view.cpp
   )

#========================================================================================
