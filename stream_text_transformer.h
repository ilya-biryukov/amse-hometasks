#ifndef STREAMTEXTTRANSFORMER_H
#define STREAMTEXTTRANSFORMER_H

#include "stream_transformer.h"

/**
 * @brief stream_transformer that doesn't do any transformation, just passes all data through.
 *      Should be used for text files.
 */
class stream_text_transformer_t : public stream_transformer_t
{
public:
  virtual void transform_to_text(std::istream & input, std::ostream & output);
};

#endif // STREAMTEXTTRANSFORMER_H