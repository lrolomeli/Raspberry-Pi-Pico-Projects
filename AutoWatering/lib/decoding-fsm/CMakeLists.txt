file(GLOB FILES decoding_fsm.c decoding_fsm.h)
add_library(decodingFSM ${FILES})
target_link_libraries(decodingFSM pico_stdlib)
target_include_directories(decodingFSM PUBLIC ./)