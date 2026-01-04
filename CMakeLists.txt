cmake_minimum_required(VERSION 3.10)
project(CFONB_Analyzer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

add_executable(CFONB_Analyzer
        src/main.c
        src/cfonb_parser.c
        src/cfonb_validator.c
        src/cfonb_stats.c
        src/cfonb_utils.c
)