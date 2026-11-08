set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(RED4apiSmokeTest Smoke.cpp)
target_link_libraries(RED4apiSmokeTest PRIVATE RED4api::Core RED4api::Transpiler)
add_test(NAME RED4apiSmokeTest COMMAND RED4apiSmokeTest)
