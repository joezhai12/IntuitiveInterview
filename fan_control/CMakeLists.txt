# Fan control
set(LIB_TARGET fan_control_lib)
set(EXE_TARGET fan_controller)

add_library(${LIB_TARGET}
  src/fan_controller.cpp
  src/fan_manager.cpp
)

target_include_directories(${LIB_TARGET}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${EXE_TARGET}
  src/main.cpp
)

target_link_libraries(${EXE_TARGET}
  ${LIB_TARGET}
  fan_config_reader
)

# install executables
install(TARGETS ${EXE_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
