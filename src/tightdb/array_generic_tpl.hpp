/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] TightDB Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/
#ifndef TIGHTDB_ARRAY_GENERIC_TPL_HPP
#define TIGHTDB_ARRAY_GENERIC_TPL_HPP

template<typename T>
inline size_t ArrayGeneric<T>::create_empty_basic_array(Allocator& alloc) 
{
    const size_t capacity = Array::initial_capacity;
    const MemRef mem_ref = alloc.Alloc(capacity);
    if (!mem_ref.pointer) 
        return 0;

    init_header(mem_ref.pointer, false, false, TDB_MULTIPLY, sizeof(T), 0, capacity);

    return mem_ref.ref;
}

template<typename T> 
inline ArrayGeneric<T>::ArrayGeneric(ArrayParent *parent, size_t ndx_in_parent, Allocator& alloc)
                               :Array(alloc)
{
    const size_t ref = create_empty_basic_array(alloc);
    if (!ref) 
        throw_error(ERROR_OUT_OF_MEMORY); // FIXME: Check that this exception is handled properly in callers
    init_from_ref(ref);
    SetParent(parent, ndx_in_parent);
    update_ref_in_parent();
}

template<typename T>
inline ArrayGeneric<T>::ArrayGeneric(size_t ref, ArrayParent *parent, size_t ndx_in_parent,
                               Allocator& alloc): Array(alloc)
{
    // Manually create array as doing it in initializer list
    // will not be able to call correct virtual functions
    init_from_ref(ref);
    SetParent(const_cast<ArrayParent *>(parent), ndx_in_parent);
}

// Creates new array (but invalid, call UpdateRef to init)
#if 0
template<typename T> 
inline ArrayGeneric<T>::ArrayGeneric(Allocator& alloc): Array(alloc) 
{
}
#endif

template<typename T> 
inline ArrayGeneric<T>::~ArrayGeneric() 
{
}


template<typename T> 
inline void ArrayGeneric<T>::Clear()
{
    CopyOnWrite();

    // Truncate size to zero (but keep capacity and width)
    m_len = 0;
    set_header_len(0);
}

template<typename T> 
inline void ArrayGeneric<T>::add(T value)
{
    Insert(m_len, value);
}

template<typename T> 
inline T ArrayGeneric<T>::Get(size_t ndx) const
{
    T* dataPtr = (T *)m_data + ndx;
    return *dataPtr;
}

template<typename T> 
inline void ArrayGeneric<T>::Set(size_t ndx, T value)
{
    TIGHTDB_ASSERT(ndx < m_len);

    // Check if we need to copy before modifying
    if (!CopyOnWrite()) 
        return;

    // Set the value
    T* data = (T *)m_data + ndx;
    *data = value;
}

template<typename T> 
void ArrayGeneric<T>::Insert(size_t ndx, T value)
{
    TIGHTDB_ASSERT(ndx <= m_len);

    // Check if we need to copy before modifying
    (void)CopyOnWrite();

    // Make room for the new value
    if (!Alloc(m_len+1, m_width))
        return;

    // Move values below insertion
    if (ndx != m_len) {
        unsigned char* src = m_data + (ndx * m_width);
        unsigned char* dst = src + m_width;
        const size_t count = (m_len - ndx) * m_width;
        memmove(dst, src, count); // FIXME: Use std::copy() or std::copy_backward() instead.
    }

    // Set the value
    T* data = (T *)m_data + ndx;
    *data = value;

     ++m_len;
}

template<typename T> 
void ArrayGeneric<T>::Delete(size_t ndx)
{
    TIGHTDB_ASSERT(ndx < m_len);

    // Check if we need to copy before modifying
    CopyOnWrite();

    --m_len;

    // move data under deletion up
    if (ndx < m_len) {
        unsigned char* src = m_data + ((ndx+1) * m_width);
        unsigned char* dst = m_data + (ndx * m_width);
        const size_t len = (m_len - ndx) * m_width;
        memmove(dst, src, len); // FIXME: Use std::copy() or std::copy_backward() instead.
    }

    // Update length in header
    set_header_len(m_len);
}

template<typename T>
bool ArrayGeneric<T>::Compare(const ArrayGeneric<T>& c) const
{
    for (size_t i = 0; i < Size(); ++i) {
        if (Get(i) != c.Get(i)) 
            return false;
    }
    return true;
}


template<typename T> 
size_t ArrayGeneric<T>::CalcByteLen(size_t count, size_t width) const
{
    // FIXME: This arithemtic could overflow. Consider using <tightdb/overflow.hpp>
    return 8 + (count * sizeof(T));
}

template<typename T> 
size_t ArrayGeneric<T>::CalcItemCount(size_t bytes, size_t width) const
{
    // ??? what about width = 0? return -1?

    const size_t bytes_without_header = bytes - 8;
    return bytes_without_header / sizeof(T);
}

template<typename T> 
size_t ArrayGeneric<T>::Find(T target, size_t start, size_t end) const
{
    TIGHTDB_ASSERT(target);

    if (end == (size_t)-1) 
        end = m_len;
    if (start == end) 
        return (size_t)-1;
    TIGHTDB_ASSERT(start < m_len && end <= m_len && start < end);
    if (m_len == 0)
        return not_found; // empty list

    for (size_t i = start; i < end; ++i) {
        if (target == Get(i)) 
            return i;
    }
    return not_found; // not found
}

template<typename T> 
size_t ArrayGeneric<T>::find_first(T value, size_t start, size_t end) const
{
    TIGHTDB_ASSERT(value);
    return Find(value, start, end);
}

template<typename T> 
void ArrayGeneric<T>::find_all(Array& result, T value, size_t add_offset, size_t start, size_t end)
{
    TIGHTDB_ASSERT(value);

    size_t first = start - 1;
    for (;;) {
        first = Find(value, first + 1, end);
        if (first != not_found)
            result.add(first + add_offset);
        else 
            break;
    }
}

template<typename T> 
size_t ArrayGeneric<T>::count(T value, size_t start, size_t end) const
{
    size_t count = 0;
    size_t lastmatch = start - 1;
    for (;;) {
        lastmatch = Find(value, lastmatch+1, end);
        if (lastmatch != not_found)
            ++count;
        else 
            break;
    }
    
    return count;
}



} // namespace tightdb

#endif // TIGHTDB_ARRAY_GENERIC_TPL_HPP
