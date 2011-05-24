#include "stream_transformer.h"

class xml_transformer_t : public stream_transformer_t
{
public:
    virtual void transform_to_text(std::istream & input, std::ostream & output);
};