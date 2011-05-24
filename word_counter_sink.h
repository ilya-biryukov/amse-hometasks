#ifndef WORD_COUNTER_SINK_H
#define WORD_COUNTER_SINK_H

#include <iosfwd>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility.hpp>

namespace details
{
class word_counter_sink_t : public boost::iostreams::sink, public boost::noncopyable
{
public:
  word_counter_sink_t();
  
  inline size_t word_count() const;
  
  std::streamsize write(char const * data, std::streamsize n);
  
  void add_delimiter(char delim);
  template<class InputIter>
  void add_delimiters(InputIter i, InputIter j);  
  void add_delimiters(std::string const & delims);
  void remove_delimiter(char delim);  
    
  void reset();
private:  
  enum state_t
  {
    STATE_WORD,
    STATE_WHITESPACE
  };

  inline bool is_delim(char c) const;
  inline void proccess_next_char(char c);
  
  boost::unordered_set<char> delimiters_;
  
  state_t state_;
  size_t word_count_;
};

// Inilne members implementation
inline  bool word_counter_sink_t::is_delim(char c) const
{
  return delimiters_.count(c) > 0;
}

void word_counter_sink_t::proccess_next_char(char c)
{
  bool delim = is_delim(c);
  if (!delim && state_ == STATE_WHITESPACE)
  {
    ++word_count_;
    state_ = STATE_WORD;
  }
  else if (delim && state_ == STATE_WORD)
  {
    state_ = STATE_WHITESPACE;
  }
}

inline size_t word_counter_sink_t::word_count() const
{
  return word_count_;
}
} // namespace details
#endif // WORD_COUNTER_SINK_H