#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <string>
#include <boost/iostreams/stream.hpp>

#include "word_counter_sink.h"

class stream_transformers_collection_t;

/**
 * @brief Class for counting words from files. 
 *      Automatically transforms files to text using transformers_collection defined in constructor
 */
class word_counter_t
{
public:
  /**
   * @brief Contructs new word_counter that will used the specified collection to query transformers
   * 
   * @param transformer_collection collection to be used for quering transformers
   */
  word_counter_t(stream_transformers_collection_t const * transformer_collection);  
  
  /**
   * @brief Counts the number of words in file.
   * 
   * @param filname path to the file
   * @returns number of words in the file
   * 
   * @throws transformer_not_found_exception when filetype doesn't have transformer for it
   * @throws file_open_failed_exception when file with filename couldn't be opened
   * @throws transformation_error_expcetion when errors occured during transformation
   */
  size_t words_in_file(std::string const & filename) const;
private:
  stream_transformers_collection_t const * transformer_collection_;
  
  mutable details::word_counter_sink_t words_counter_sink_;  
};

#endif // WORD_COUNTER_H