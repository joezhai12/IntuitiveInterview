# Subsystem
set(LIB_TARGET fan_config_reader)

add_library(${LIB_TARGET}
  src/fan_config_reader.cpp
)

target_include_directories(${LIB_TARGET}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)