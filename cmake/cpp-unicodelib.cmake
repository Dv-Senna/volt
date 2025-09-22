add_library(cpp-unicodelib INTERFACE)
add_library(cpp-unicodelib::cpp-unicodelib ALIAS cpp-unicodelib)
target_include_directories(cpp-unicodelib INTERFACE ${PROJECT_SOURCE_DIR}/vendors/cpp-unicodelib)
target_compile_features(cpp-unicodelib INTERFACE cxx_std_17)
