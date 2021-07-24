cmake_minimum_required(VERSION 3.0)

# project name and languages
project(TinyDebugger LANGUAGES CXX)

# td.exe
add_executable(td
    dat.cpp
    main.cpp
    MR.cpp
    TinyD.cpp
    Utils.cpp)
target_compile_definitions(td PRIVATE UNICODE _UNICODE)
target_link_libraries(td PRIVATE shlwapi)

# msgbox.exe
add_executable(msgbox msgbox.cpp)

# spec2dat.exe
add_executable(spec2dat
    dat.cpp
    spec2dat/spec.cpp
    spec2dat/spec2dat.cpp)
target_include_directories(spec2dat PRIVATE .)
target_link_libraries(spec2dat PRIVATE shlwapi)

# dat_limited.exe
add_executable(dat_limited
    dat.cpp
    MR.cpp
    dat_limited/dat_limited.cpp)
target_include_directories(dat_limited PRIVATE .)
target_link_libraries(dat_limited PRIVATE shlwapi)
