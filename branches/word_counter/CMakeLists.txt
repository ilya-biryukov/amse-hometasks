cmake_minimum_required (VERSION 2.6)
project (word_counter)

set (word_counter_SRCS
  main.cpp
  stream_transformer.cpp
  stream_text_transformer.cpp
  xml_transformer.cpp
  stream_transformers_collection.cpp
  expat_parser.cpp
  word_counter_sink.cpp
  word_counter.cpp
)

set (word_counter_LIBS
)

find_package (Boost COMPONENTS filesystem REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
set (word_counter_LIBS ${word_counter_LIBS} ${Boost_LIBRARIES})

find_package (EXPAT REQUIRED)
include_directories (EXPAT_INCLUDE_DIRS)
set (word_counter_LIBS ${word_counter_LIBS} ${EXPAT_LIBRARIES})

add_executable (word_counter ${word_counter_SRCS})

target_link_libraries (word_counter ${word_counter_LIBS})

