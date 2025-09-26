# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appImagePro_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appImagePro_autogen.dir/ParseCache.txt"
  "appImagePro_autogen"
  )
endif()
