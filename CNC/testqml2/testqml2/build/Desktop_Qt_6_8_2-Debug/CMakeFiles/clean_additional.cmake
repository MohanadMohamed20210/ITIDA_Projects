# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/apptestqml2_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/apptestqml2_autogen.dir/ParseCache.txt"
  "apptestqml2_autogen"
  )
endif()
