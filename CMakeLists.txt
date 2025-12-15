cmake_minimum_required(VERSION 3.28.1)
project(CFONB_Analyzer C)

set(CMAKE_C_STANDARD 11)

add_executable(CFONB_Analyzer main.c
        src/cfonb_parser.h.c
        src/cfonb_validator.c
        src/cfonb_stats.c
        src/cfonb_utils.c
        include/cfonb_types.h
        include/cfonb_parser.h
        src/main.c
        include/cfonb_stats.h
        include/cfonb_utils.h
        include/cfonb_validator.h)
