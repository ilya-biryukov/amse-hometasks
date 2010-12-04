#include "largeint.h"

class ReferenceCounted
{
public:
    explicit ReferenceCounted(LargeInteger * ptr);
    void ref();
    void unref();

    LargeInteger * get_value();
private:
    ~ReferenceCounted();
    ReferenceCounted(const ReferenceCounted &);
    ReferenceCounted & operator = (const ReferenceCounted &);
    LargeInteger * m_value_ptr;
    size_t     m_counter;
};

class SharedPtr
{
public:
    SharedPtr(LargeInteger * ptr);
    SharedPtr(const SharedPtr& ptr);
    ~SharedPtr();
    SharedPtr & operator = (const SharedPtr & rhs);

    void swap(SharedPtr & ptr);
    void reset(LargeInteger * val);
    LargeInteger * get_pointer() const;

    LargeInteger * operator -> ();
    LargeInteger & operator * ();
private:
    ReferenceCounted * m_ref_counted;
};

bool operator <  (const LargeInteger * lhs, const SharedPtr & rhs);
bool operator <= (const LargeInteger * lhs, const SharedPtr & rhs);
bool operator >  (const LargeInteger * lhs, const SharedPtr & rhs);
bool operator >= (const LargeInteger * lhs, const SharedPtr & rhs);
bool operator == (const LargeInteger * lhs, const SharedPtr & rhs);

bool operator <  (const SharedPtr & lhs, const SharedPtr & rhs);
bool operator <= (const SharedPtr & lhs, const SharedPtr & rhs);
bool operator >  (const SharedPtr & lhs, const SharedPtr & rhs);
bool operator >= (const SharedPtr & lhs, const SharedPtr & rhs);
bool operator == (const SharedPtr & lhs, const SharedPtr & rhs);

bool operator <  (const SharedPtr & lhs, const LargeInteger * rhs);
bool operator <= (const SharedPtr & lhs, const LargeInteger * rhs);
bool operator >  (const SharedPtr & lhs, const LargeInteger * rhs);
bool operator >= (const SharedPtr & lhs, const LargeInteger * rhs);
bool operator == (const SharedPtr & lhs, const LargeInteger * rhs);

