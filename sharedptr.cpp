#include "sharedptr.h"

/* Reference counted */
ReferenceCounted::ReferenceCounted(LargeInteger * ptr)
    : m_value_ptr(ptr),
      m_counter(1)
{ }

ReferenceCounted::~ReferenceCounted()
{ }

void ReferenceCounted::ref()
{
    ++m_counter;
}

void ReferenceCounted::unref()
{
    if (!(--m_counter))
    {
	delete m_value_ptr;
	delete this; // мы используем его только внутри своего класса, так что такой подход оправдан
    }
}

LargeInteger * ReferenceCounted::get_value()
{
    return m_value_ptr;
}

/* SharedPtr */
SharedPtr::SharedPtr(LargeInteger * ptr)
    : m_ref_counted(new ReferenceCounted(ptr))
{ }

SharedPtr::~SharedPtr()
{
    m_ref_counted->unref();
}

SharedPtr::SharedPtr(const SharedPtr& ptr)
    : m_ref_counted(ptr.m_ref_counted)
{
    m_ref_counted->ref();
}

SharedPtr & SharedPtr::operator = (const SharedPtr & rhs)
{
    if (this != &rhs)
    {
	SharedPtr(rhs).swap(*this);
    }

    return *this;
}

void SharedPtr::swap(SharedPtr & ptr)
{
    std::swap(m_ref_counted, ptr.m_ref_counted);
}

void SharedPtr::reset(LargeInteger * val)
{
    m_ref_counted->unref();
    m_ref_counted = new ReferenceCounted(val);
}

LargeInteger * SharedPtr::get_pointer() const
{
    return m_ref_counted->get_value();
}

LargeInteger * SharedPtr::operator -> ()
{
    return m_ref_counted->get_value();
}

LargeInteger & SharedPtr::operator * ()
{
    return *m_ref_counted->get_value();
}

bool operator < (const LargeInteger * lhs, const SharedPtr & rhs)
{
    return lhs < rhs.get_pointer();
}

bool operator <= (const LargeInteger * lhs, const SharedPtr & rhs)
{
    return lhs <= rhs.get_pointer();
}

bool operator > (const LargeInteger * lhs, const SharedPtr & rhs)
{
    return lhs > rhs.get_pointer();
}

bool operator >=  (const LargeInteger * lhs, const SharedPtr & rhs)
{
    return lhs >= rhs.get_pointer();
}

bool operator == (const LargeInteger * lhs, const SharedPtr & rhs)
{
    return lhs == rhs.get_pointer();
}

bool operator < (const SharedPtr & lhs, const SharedPtr & rhs)
{
    return lhs.get_pointer() < rhs.get_pointer();
}

bool operator <= (const SharedPtr & lhs, const SharedPtr & rhs)
{
    return lhs.get_pointer() <= rhs.get_pointer();
}

bool operator > (const SharedPtr & lhs, const SharedPtr & rhs)
{
    return lhs.get_pointer() > rhs.get_pointer();
}

bool operator >= (const SharedPtr & lhs, const SharedPtr & rhs)
{
    return lhs.get_pointer() >= rhs.get_pointer();
}

bool operator == (const SharedPtr & lhs, const SharedPtr & rhs)
{
    return lhs.get_pointer() == rhs.get_pointer();
}

bool operator <  (const SharedPtr & lhs, const LargeInteger * rhs)
{
    return lhs.get_pointer() < rhs;
}

bool operator <=  (const SharedPtr & lhs, const LargeInteger * rhs)
{
    return lhs.get_pointer() <= rhs;
}

bool operator >  (const SharedPtr & lhs, const LargeInteger * rhs)
{
    return lhs.get_pointer() > rhs;
}

bool operator >=  (const SharedPtr & lhs, const LargeInteger * rhs)
{
    return lhs.get_pointer() >= rhs;
}

bool operator == (const SharedPtr & lhs, const LargeInteger * rhs)
{
    return lhs.get_pointer() == rhs;
}
