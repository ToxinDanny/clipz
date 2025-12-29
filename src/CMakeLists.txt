add_executable (clipz "program.c" "program.h" "memoryarena.h" "hotkeymap.h" "memoryarena.c" "hotkeymap.c" "utils.h" "utils.c"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET clipz PROPERTY C 11)
endif()
