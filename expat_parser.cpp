#include "expat_parser.h"

#include <boost/scoped_array.hpp>

#include "consts.h"
#include "exceptions.h"

namespace
{
  struct handlers_t
  {
    details::expat_parser_t::start_element_handler_f start_handler;
    details::expat_parser_t::end_element_handler_f   end_handler;
    details::expat_parser_t::char_data_handler_f     char_handler;
  };
  
  static void handle_start_element(void * data, char const * name, char const ** attrs)
  {
    handlers_t * handlers = static_cast<handlers_t *>(data);
    handlers->start_handler(name, attrs);
  }
  
  static void handle_end_element(void * data, char const * name)
  {
    handlers_t * handlers = static_cast<handlers_t *>(data);
    handlers->end_handler(name);
  }
  
  static void handle_char_data(void * data, char const * char_data, int len)
  {
    handlers_t * handlers = static_cast<handlers_t *>(data);
    handlers->char_handler(char_data, len);
  }
} // namespace
namespace details
{
expat_parser_t::expat_parser_t()
  : parser_(XML_ParserCreate(0))
{
}

expat_parser_t::~expat_parser_t()
{
  XML_ParserFree(parser_);
}

void expat_parser_t::parse_stream(std::istream & input, 
                                  expat_parser_t::start_element_handler_f start_handler, 
                                  expat_parser_t::end_element_handler_f end_handler, 
                                  expat_parser_t::char_data_handler_f char_data)
{
  handlers_t handlers;
  handlers.start_handler = start_handler;
  handlers.end_handler = end_handler;
  handlers.char_handler = char_data;
  
  XML_SetUserData(parser_, &handlers);
  XML_SetStartElementHandler(parser_, &handle_start_element);
  XML_SetEndElementHandler(parser_, &handle_end_element);
  XML_SetCharacterDataHandler(parser_, &handle_char_data);
  
  size_t buffer_size = TEXT_BUFFER_DEFAULT_SIZE;
  boost::scoped_array<char> buffer(new char[buffer_size]);
  while (!input.eof())
  {
    input.read(buffer.get(), buffer_size);
    
    XML_Status status = XML_Parse(parser_, buffer.get(), input.gcount(), false);
    if (status == XML_STATUS_ERROR)
    {
      throw transformation_error_expcetion(XML_ErrorString(XML_GetErrorCode(parser_)));
    }
  }
  XML_Parse(parser_, 0, 0, true);
}

} // namespace details