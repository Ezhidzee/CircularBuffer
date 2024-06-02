#ifndef CIRCULARBUFFER_CCIRCULARBUFFEREXT_HPP
#define CIRCULARBUFFER_CCIRCULARBUFFEREXT_HPP

#include "CCircularBuffer.hpp"

template<typename T, typename Alloc = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T, Alloc> {
public:
    typedef typename CCircularBuffer<T, Alloc>::value_type value_type;

    typedef typename CCircularBuffer<T, Alloc>::size_type size_type;

    typedef typename CCircularBuffer<T, Alloc>::iterator iterator;

    typedef typename CCircularBuffer<T, Alloc>::pointer pointer;

    explicit CCircularBufferExt(Alloc alloc = Alloc()) { CCircularBuffer<T, Alloc>(); }

    CCircularBufferExt(const CCircularBufferExt &c, Alloc alloc = Alloc()) : CCircularBuffer<T, Alloc>(c, alloc) {}

    explicit CCircularBufferExt(size_type n, Alloc alloc = Alloc()) : CCircularBuffer<T, Alloc>(n, alloc) {}

    CCircularBufferExt(size_type n, value_type t, Alloc alloc = Alloc()) : CCircularBuffer<T, Alloc>(n, t, alloc) {}

    CCircularBufferExt(iterator i, iterator j, Alloc alloc = Alloc()) : CCircularBuffer<T, Alloc>(i, j, alloc) {}

    CCircularBufferExt(std::initializer_list<value_type> il, Alloc alloc = Alloc()) : CCircularBuffer<T, Alloc>(il,
                                                                                                                alloc) {}

    iterator insert(iterator p, value_type t) {
        size_type n = p - this->begin();
        if (this->bufferSize >= this->bufferMaxSize) {
            Alloc alloc;
            pointer newData = alloc.allocate((this->bufferMaxSize * 2) + 1);
            this->bufferMaxSize *= 2;
            for (size_type i = 0; i < this->bufferSize + 1; ++i) alloc.construct(&newData[i]);
            for (size_type i = 0; i < this->bufferSize; ++i) newData[i] = this->data[i];
            std::_Destroy_n(this->data, (this->bufferMaxSize / 2) + 1);
            alloc.deallocate(this->data, (this->bufferMaxSize / 2) + 1);
            this->data = newData;
        }

        ++(this->bufferSize);
        ++(this->endIndex);
        for (size_type i = this->bufferSize - 1; i > n; --i) {
            this->data[i] = this->data[i - 1];
        }
        this->data[n] = t;
        return this->begin() + n;
    }

    iterator insert(iterator p, size_type n, value_type t) {
        for (size_type i = 0; i < n && i < this->bufferMaxSize; ++i) insert(p, t);
        return iterator(p - 1);
    }

    iterator insert(iterator p, iterator i, iterator j) {
        if (i > j) throw std::length_error("The first iterator is greater than the second");
        size_type n = j - i;
        if (n == 0) return p;
        n %= this->bufferMaxSize;
        for (size_type k = 1; k < n + 1; ++k) insert(p, *(j - k));

        return iterator(p - 1);
    }

    iterator insert(iterator p, std::initializer_list<value_type> il) {
        size_type n = p - this->begin();
        if (il.size() == 0 || p == this->begin()) return p;
        for (size_type i = 0, j = 0; i < il.size() && j < this->bufferMaxSize; ++i, ++j) {
            insert(p, *(il.begin() + i));
            p = this->begin() + ++n;
        }
        return iterator(p - 1);
    }

    void push_front(value_type t) {
        if (this->bufferSize >= this->bufferMaxSize) {
            Alloc alloc;
            pointer newData = alloc.allocate((this->bufferMaxSize * 2) + 1);
            this->bufferMaxSize *= 2;
            for (size_type i = 0; i < this->bufferSize; ++i) alloc.construct(&newData[i]);
            for (size_type i = 0; i < this->bufferSize; ++i) newData[i] = this->data[i];
            std::_Destroy_n(this->data, (this->bufferMaxSize / 2) + 1);
            alloc.deallocate(this->data, (this->bufferMaxSize / 2) + 1);
            this->data = newData;
        }
        Alloc alloc;
        alloc.construct(&this->data[(this->bufferSize)++]);

        for (size_type i = this->bufferSize; i >= 1; --i) this->data[i] = this->data[i - 1];
        ++(this->endIndex);
        this->data[0] = t;
    }

    void push_back(value_type t) {
        if (this->bufferSize >= this->bufferMaxSize) {
            Alloc alloc;
            pointer newData = alloc.allocate((this->bufferMaxSize * 2) + 1);
            this->bufferMaxSize *= 2;
            for (size_type i = 0; i < this->bufferSize; ++i) alloc.construct(&newData[i]);
            for (size_type i = 0; i < this->bufferSize; ++i) newData[i] = this->data[i];
            std::_Destroy_n(this->data, (this->bufferMaxSize / 2) + 1);
            alloc.deallocate(this->data, (this->bufferMaxSize / 2) + 1);
            this->data = newData;
        }
        Alloc alloc;
        alloc.construct(&this->data[this->bufferSize]);

        this->data[this->bufferSize] = t;
        ++(this->bufferSize);
        ++(this->endIndex);
    }
};

#endif