#include "stream_text_transformer.h"

#include <boost/scoped_array.hpp>

#include "consts.h"

void stream_text_transformer_t::transform_to_text(std::istream & input, std::ostream & output)
{
  if (input.eof())
    return;
   
  size_t buffer_size = TEXT_BUFFER_DEFAULT_SIZE;
  boost::scoped_array<char> buffer(new char[buffer_size]);
  
  while (!input.eof())
  {
    input.read(buffer.get(), buffer_size);        
    size_t chars_read = input.gcount();

    output.write(buffer.get(), chars_read);
  }
}
