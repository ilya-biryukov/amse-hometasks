#include "word_counter_sink.h"

#include <boost/bind.hpp>
namespace details
{
std::streamsize word_counter_sink_t::write(char const * data, std::streamsize n)
{
  std::for_each(data, data + n, boost::bind(&word_counter_sink_t::proccess_next_char, this, _1));
  return n;
}

word_counter_sink_t::word_counter_sink_t()
  : word_count_(0), state_(STATE_WHITESPACE)
{
}

void word_counter_sink_t::add_delimiter(char delim)
{
  delimiters_.insert(delim);
}

void word_counter_sink_t::remove_delimiter(char delim)
{
  boost::unordered_set<char>::iterator it = delimiters_.find(delim);
  if (it != delimiters_.end())
  {
    delimiters_.erase(it);
  }
}

template<class InputIter>
void word_counter_sink_t::add_delimiters(InputIter i, InputIter j)
{
  delimiters_.insert(i, j);
}

void word_counter_sink_t::add_delimiters(std::string const & delims)
{  
  add_delimiters(delims.begin(), delims.end());
}

void word_counter_sink_t::reset()
{
  state_ = STATE_WHITESPACE;
  word_count_ = 0;
}
} // namespace details