#ifndef STREAMTRANSFORMER_H
#define STREAMTRANSFORMER_H

#include <istream>
#include <ostream>

#include "exceptions.h"

/**
 * @brief Abstract class that represents objects that are used for transforming any input into text
 *      Subclasses must implement transform_to_text and make transformation of input data into text there.
 */
class stream_transformer_t
{
public:
    virtual ~stream_transformer_t();
   
    /**
     * @brief transform the data from input stream into text output.
     * 
     * @param input input stream, where to read data from. Should be read until the eof.
     * @param output output stream, where to write text.
     * 
     * @throws transformation_error_exception to report errors during transformation or badly formated input file
     */
    virtual void transform_to_text(std::istream & input, std::ostream & output) = 0;
};

#endif // STREAMTRANSFORMER_H
