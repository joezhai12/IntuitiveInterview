set(LIB_TARGET serialization)

add_library(${LIB_TARGET}
  src/serialization.cpp
)

target_include_directories(${LIB_TARGET}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)