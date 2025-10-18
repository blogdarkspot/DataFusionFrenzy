# Defina a versão mínima do CMake
cmake_minimum_required(VERSION 3.20)
# Ativar o uso de testes com CTest
enable_testing()

# Forçar a versão mínima de CMake para o GoogleTest
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0  # A versão pode ser alterada conforme necessário
)
# Baixar e adicionar o GoogleTest como subdiretório
FetchContent_MakeAvailable(googletest)
add_executable(test conflation_tests.cpp)
# Linkar o executável de teste com o GoogleTest e a biblioteca UMDFLib
target_link_libraries(test PUBLIC gtest gtest_main Conflation)
