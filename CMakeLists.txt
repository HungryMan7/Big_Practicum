project (custom_lang)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories (custom_lang/resources)

file(GLOB ALL_FILES
     custom_lang/*.cpp
	 custom_lang/backend/includes/*.h
	 custom_lang/backend/source/*.cpp
	 custom_lang/resources/*.txt
)

add_executable (main ${ALL_FILES})