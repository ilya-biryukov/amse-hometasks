#include "stream_transformers_collection.h"

#include <utility>

#include "exceptions.h"
#include "utils.h"

void stream_transformers_collection_t::add_transformer(std::string const & type, 
                                                       boost::shared_ptr<stream_transformer_t> transformer)
{
  std::string ltype(type); // type in lower case
  transform_to_lower(ltype);
  
  transformers_.insert(std::make_pair(ltype, transformer));
}

boost::shared_ptr<stream_transformer_t> stream_transformers_collection_t::transformer_for_type(std::string const & type) const
{
  std::string ltype(type); // type in lower case
  transform_to_lower(ltype);  
  
  transformers_map_t::const_iterator it = transformers_.find(ltype);
  if (it != transformers_.end())
  {
    return it->second;
  }
  else
  {
    throw transformer_not_found_exception();
  }
}