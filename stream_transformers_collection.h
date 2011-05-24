#ifndef STREAMTRANSFORMERCOLLECTION_H
#define STREAMTRANSFORMERCOLLECTION_H

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "stream_transformer.h"

/**
 * @brief Represents collection of transformers. Transformers for specified file types can be added and queried.
 */
class stream_transformers_collection_t
{
public:  
  /**
   * @brief Adds transformer to collection for type. If transformer for this type is already there it is replaced
   * 
   * @param type type of file to be transformed
   * @param transformer the transformer to be speciefied for the filetype
   */
  void add_transformer(std::string const & type, 
                       boost::shared_ptr<stream_transformer_t> transformer);  
  /**
   * @brief Find transformer for the specified type
   * 
   * @param type type of file to look transformer for
   * 
   * @throws transformer_not_found_exception when no transformer for the specified files was registred.
   */ 
  boost::shared_ptr<stream_transformer_t> transformer_for_type(std::string const & type) const;  
private:  
  typedef boost::unordered_map<std::string, boost::shared_ptr<stream_transformer_t> > transformers_map_t;
  
  transformers_map_t transformers_;
};

#endif // STREAMTRANSFORMERCOLLECTION_H