#include <string>
#include <algorithm>
#include <locale>

#include <boost/bind.hpp>

/**
 * @brief Transforms string to lower case.
 * 
 * @param str string to be lower cased
 */
inline void transform_to_lower(std::string & str)
{
  std::transform(str.begin(), 
                 str.end(), 
                 str.begin(), 
                 boost::bind(&std::tolower<std::string::value_type>, _1, std::locale()));
}