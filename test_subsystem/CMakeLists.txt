# Subsystem
set(EXE_TARGET subsystem)

# target_include_directories(${EXE_TARGET}
#   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

add_executable(${EXE_TARGET}
  src/main.cpp
)

target_link_libraries(${EXE_TARGET}
  serialization
)

# install executables
install(TARGETS ${EXE_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

if(BUILD_UNIT_TESTS)
  add_subdirectory(test)
endif()