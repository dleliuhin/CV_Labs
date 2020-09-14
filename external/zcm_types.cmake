#========================================================================================

include_directories( ${CMAKE_CURRENT_LIST_DIR}/zcm_types
                     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/service
                     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/service/cpp_types
                     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/camera_basler
                     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/camera_basler/cpp_types )

#========================================================================================

set( ZCM_TYPES
     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/service/cpp_types/ZcmService.hpp
     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/camera_basler/cpp_types/ZcmCameraBaslerFrame.hpp
     ${CMAKE_CURRENT_LIST_DIR}/zcm_types/camera_basler/cpp_types/ZcmCameraBaslerJpegFrame.hpp )

#========================================================================================
