############### LIB ########################################################

set(LIB_NAME DataFusionFrenzy)

add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${LIB_NAME} INTERFACE cxx_std_14)

add_library(dff::dataff ALIAS ${LIB_NAME})