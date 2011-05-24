#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class word_counter_exception : public std::exception
{
};

class transformer_not_found_exception : public word_counter_exception
{
};

class file_open_failed_exception: public word_counter_exception
{
};

class transformation_error_expcetion : public word_counter_exception
{
public:
  transformation_error_expcetion(char const * message);
  virtual char const * what() const throw();
private:
  char const * message_;
};

// Inline members implementation

inline char const * transformation_error_expcetion::what() const throw()
{
  return message_;
}

// Inline constructor mighe not be a good idea
inline transformation_error_expcetion::transformation_error_expcetion(char const * message) 
  : message_(message)
{
}


#endif // EXCEPTIONS_H