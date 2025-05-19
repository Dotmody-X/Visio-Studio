# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/VISIO_STUDIO_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/VISIO_STUDIO_autogen.dir/ParseCache.txt"
  "VISIO_STUDIO_autogen"
  )
endif()
