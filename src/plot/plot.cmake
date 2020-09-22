#========================================================================================

find_package( OpenCV REQUIRED )

#========================================================================================

include_directories( ${CMAKE_CURRENT_LIST_DIR} )

#========================================================================================

set( PLOT
    ${CMAKE_CURRENT_LIST_DIR}/view.h
    ${CMAKE_CURRENT_LIST_DIR}/view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/form.cpp
    ${CMAKE_CURRENT_LIST_DIR}/form.h
    ${CMAKE_CURRENT_LIST_DIR}/form.ui
   )

#========================================================================================
