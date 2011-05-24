#include <algorithm>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "exceptions.h"
#include "word_counter.h"
#include "stream_transformers_collection.h"
#include "stream_text_transformer.h"
#include "xml_transformer.h"
#include "stream_transformer.h"

/**
 * @brief Initializes collection with transformers.
 *      If you add new file format, you should probably add few lines of code that
 *      will add your transformer to collection
 * @param collection transformers_collection to be initialized
 */
void init_transformers_collection(stream_transformers_collection_t * collection)
{
  collection->add_transformer(".txt", boost::shared_ptr<stream_transformer_t>(new stream_text_transformer_t()));
  collection->add_transformer(".xml", boost::shared_ptr<stream_transformer_t>(new xml_transformer_t()));
}

/**
 * @brief Counts words using counter and outputs result to console. Handles all errors 
 * 
 * @param counter object that is used to count words
 * @param file the name of the file to count from
 */
void count_file(word_counter_t const & counter, std::string const & filename)
{
  std::cout << filename << ": ...";
  std::cout.flush();
  
  try
  {
    size_t total_words = counter.words_in_file(filename);
    std::cout << "\r" << filename << ": " << total_words << " words." << std::endl;
  }
  catch (file_open_failed_exception & e)
  {
    std::cout << "\r" << filename << ": file opening failed." << std::endl;    
  }
  catch (transformer_not_found_exception & e)
  {
    std::cout << "\r" << filename << ": file format not supported." << std::endl;
  }
  catch (word_counter_exception & e)
  {
    std::cout << "\r" << filename << ": unknown error while counting words." << std::endl;
  }
  catch (...)
  {
    std::cout << "\r" << filename << ": unknown error." << std::endl;
  }
}

int main(int argc, char ** argv) 
{  
  size_t files_count = argc - 1;
  if (files_count == 0)
  {
    std::cout << "Usage: word_counter file1 [file2] [file3] ... [fileN]" << std::endl;
    return 0;
  }
  
  stream_transformers_collection_t collection;
  init_transformers_collection(&collection);
  
  word_counter_t word_counter(&collection);
  
  
  std::cout << "Total files: " << files_count << std::endl;  
  std::for_each(argv + 1, argv + 1 + files_count, boost::bind(&count_file, boost::cref(word_counter), _1));
}
