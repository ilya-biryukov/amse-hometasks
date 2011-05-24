#ifndef EXPAT_PARSER_H
#define EXPAT_PARSER_H

#include <boost/function.hpp>
#include <expat.h>

namespace details
{
class expat_parser_t
{
public:
  // First parameter is the name of the element. The second is attributes. Last attribute is zero-pointer.
  typedef boost::function<void(char const *, char const **)> start_element_handler_f;
  // First parameter is the name of the element.
  typedef boost::function<void(char const *)> end_element_handler_f;
  // First parameter is the chars data, second is it's length
  typedef boost::function<void(char const *, size_t)> char_data_handler_f;
  
  expat_parser_t();
  ~expat_parser_t();
  
  // None of the handlers must throw exceptions
  void parse_stream(std::istream & input, start_element_handler_f start_handler, end_element_handler_f end_handler, char_data_handler_f char_data);
private:
  XML_Parser parser_;  
};
} // namespace details
#endif // EXPAT_PARSER_H