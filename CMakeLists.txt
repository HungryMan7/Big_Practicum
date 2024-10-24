project (custom-lang)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB ALL_FILES
     custom_lang/*.cpp
	 custom_lang/backend/includes/*.h
	 custom_lang/backend/source/*.cpp
)

add_executable (main ${ALL_FILES})