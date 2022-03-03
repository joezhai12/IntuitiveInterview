# Subsystem
set(EXE_TARGET subsystem)

add_executable(${EXE_TARGET}
  src/main.cpp
  src/subsystem.cpp
)

target_include_directories(${EXE_TARGET}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# install executables
install(TARGETS ${EXE_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
