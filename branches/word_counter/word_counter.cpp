#include "word_counter.h"

#include <assert.h>

#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/ref.hpp>

#include "exceptions.h"
#include "stream_transformers_collection.h"

namespace boostio = boost::iostreams;
namespace boostfs = boost::filesystem3;

word_counter_t::word_counter_t(stream_transformers_collection_t const * transformer_collection)
  : transformer_collection_(transformer_collection)
{
  words_counter_sink_.add_delimiters(" ~!@#$%^&*()_+~`[]{}\"'?,.><\\|+-/\n\r");
}

size_t word_counter_t::words_in_file(std::string const & filename) const
{
  boostfs::path extension = boostfs::path(filename).extension();
  
  boost::shared_ptr<stream_transformer_t> transformer = 
    transformer_collection_->transformer_for_type(extension.string());
  
  std::ifstream input(filename.c_str());  
  if (!input.is_open())
  {
    throw file_open_failed_exception();
  }
   
  typedef boost::reference_wrapper<details::word_counter_sink_t> sink_wrapper_t;
  boostio::stream<sink_wrapper_t> output(boost::ref(words_counter_sink_), 0, 0);
  assert(output.component()->get_pointer() == &words_counter_sink_);
  
  transformer->transform_to_text(input, output);
  
  size_t word_count = words_counter_sink_.word_count();
  words_counter_sink_.reset();
  
  return word_count;
}


