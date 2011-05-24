#include "xml_transformer.h"

#include <cstring>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "expat_parser.h"


namespace
{
  void write_to_ostream_with_len(char const * data, size_t len);
  void write_to_ostream(char const * data);
  
  void handle_start_element(std::ostream & output, char const * name, char const ** attrs)
  {
    for (char const ** attr = attrs; *attr != 0; attr += 2)
    {
      output.write(attr[1], std::strlen(attr[1]));
    }
  }
  
  void handle_end_element(std::ostream & output, char const * name)
  {
/*    output.write(name, std::strlen(name));    */
  }
  
  void handle_char_data(std::ostream & output, char const * data, int len)
  {
    output.write(data, len);
  }
} // namespace

void xml_transformer_t::transform_to_text(std::istream & input, std::ostream & output)
{
  details::expat_parser_t().parse_stream(input, 
                                         boost::bind(&handle_start_element, boost::ref(output), _1, _2),
                                         boost::bind(&handle_end_element,   boost::ref(output), _1),
                                         boost::bind(&handle_char_data,     boost::ref(output), _1, _2)
                                        );
}
