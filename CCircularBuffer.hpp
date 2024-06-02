#ifndef CIRCULARBUFFER_CCIRCULARBUFFER_HPP
#define CIRCULARBUFFER_CCIRCULARBUFFER_HPP

#include <iostream>
#include <memory>
#include <limits>
#include <initializer_list>

template<typename T, typename Alloc = std::allocator<T>>
class CCircularBuffer {
protected:
    T *data;

    size_t bufferSize;

    size_t bufferMaxSize;

    size_t beginIndex;

    size_t endIndex;

public:
    template<typename _iterator>
    class normal_iterator {
    private:
        _iterator it;

        _iterator memoryBegin;

        _iterator memoryEnd;

        _iterator dataBegin;

        _iterator dataEnd;

        typedef std::iterator_traits<_iterator> traits_type;

    public:
        friend class CCircularBuffer<T, Alloc>;

        typedef typename traits_type::value_type value_type;

        typedef typename traits_type::reference reference;

        typedef typename traits_type::pointer pointer;

        typedef typename traits_type::difference_type difference_type;

        typedef std::random_access_iterator_tag iterator_category;

        //DefaultConstructible
        normal_iterator() = default;

        //CopyConstructible
        normal_iterator(const normal_iterator &v) {
            it = v.it;
            memoryBegin = v.memoryBegin;
            memoryEnd = v.memoryEnd;
            dataBegin = v.dataBegin;
            dataEnd = v.dataEnd;
        }

        normal_iterator(_iterator _it, _iterator _memoryBegin, _iterator _memoryEnd, _iterator _dataBegin,
                        _iterator _dataEnd) {
            it = _it;
            memoryBegin = _memoryBegin;
            memoryEnd = _memoryEnd;
            dataBegin = _dataBegin;
            dataEnd = _dataEnd;
        }

        //Destructible
        ~normal_iterator() = default;

        normal_iterator &operator++() {
            if (it == dataEnd) {
                it = memoryEnd;
            } else if (it == memoryEnd - 1 && dataBegin > dataEnd) {
                it = memoryBegin;
            } else if (it == memoryBegin - 1) {
                it = dataBegin;
            } else ++it;
            return *this;
        }

        normal_iterator operator++(int) {
            normal_iterator retVal = *this;
            ++(*this);
            return retVal;
        }

        normal_iterator &operator--() {
            if (it == dataBegin) {
                it = memoryBegin - 1;
            } else if (it == memoryEnd) {
                it = dataEnd;
            } else if (it == memoryBegin && memoryBegin != dataBegin) {
                it = memoryEnd - 1;
            } else --it;
            return *this;
        }

        normal_iterator operator--(int) {
            normal_iterator retVal = *this;
            --(*this);
            return retVal;
        }

        //EqualityComparable
        bool operator==(const normal_iterator &v) {
            return it == v.it;
        }

        bool operator!=(const normal_iterator &v) {
            return it != v.it;
        }

        normal_iterator &operator+=(difference_type n) {
            if (n < 0) return (*this) -= (-n);
            if (it == dataEnd || it >= memoryEnd) {
                if (it == dataEnd) {
                    it = memoryEnd;
                    --n;
                }
                it += n;
                return *this;
            } else if (it < memoryBegin) {
                if (n <= memoryBegin - 1 - it) {
                    it += n;
                    return *this;
                } else {
                    n -= memoryBegin - 1 - it + 1;
                    it = dataBegin;
                }
            }
            if (it >= dataBegin && it > dataEnd) {
                n += it - dataBegin;
            } else if (it <= dataEnd && it < dataBegin) {
                n += memoryEnd - dataBegin + (it - memoryBegin);
            } else if (it <= dataEnd && it >= dataBegin) {
                n += (it - dataBegin);
            }
            if (n >= memoryEnd - memoryBegin) {
                it = memoryEnd + n - (memoryEnd - memoryBegin);
                return *this;
            }
            if (n > memoryEnd - dataBegin - 1) {
                it = memoryBegin + n - (memoryEnd - dataBegin);
            } else it = dataBegin + n;
            return *this;
        }

        normal_iterator &operator-=(difference_type n) {
            if (n < 0) return (*this) += (-n);
            if (it >= memoryEnd) {
                if (n <= it - memoryEnd) {
                    it -= n;
                    return *this;
                } else {
                    n -= it - memoryEnd + 1;
                    it = dataEnd;
                }
            } else if (it == dataBegin || it < memoryBegin) {
                if (it == dataBegin) {
                    it = memoryBegin - 1;
                    --n;
                }
                it -= n;
                return *this;
            }
            if (it >= dataBegin && it > dataEnd) {
                n += memoryEnd - it + (dataEnd - memoryBegin);
            } else if (it <= dataEnd && it < dataBegin) {
                n += (dataEnd - it);
            } else if (it <= dataEnd && it >= dataBegin) {
                n += (dataEnd - it);
            }
            if (n >= memoryEnd - memoryBegin) {
                it = memoryBegin - 1 - n + (memoryEnd - memoryBegin);
                return *this;
            }
            if (n > dataEnd - memoryBegin) {
                it = memoryEnd - n + (dataEnd - memoryBegin);
            } else it = dataEnd - n;
            return *this;
        }

        //CopyAssignable
        normal_iterator &operator=(const normal_iterator &v) {
            it = v.it;
            memoryBegin = v.memoryBegin;
            memoryEnd = v.memoryEnd;
            dataBegin = v.dataBegin;
            dataEnd = v.dataEnd;
            return *this;
        }

        normal_iterator operator+(difference_type n) {
            normal_iterator retVal = *this;
            retVal += n;
            return retVal;
        }

        friend normal_iterator operator+(difference_type n, const normal_iterator &v) {
            normal_iterator retVal = v;
            return retVal + n;
        }

        normal_iterator operator-(difference_type n) {
            normal_iterator retVal = *this;
            retVal -= n;
            return retVal;
        }

        difference_type operator-(const normal_iterator &v) const {
            if (it <= memoryEnd && it >= memoryBegin && v.it <= memoryEnd && v.it >= memoryBegin) {
                if (dataEnd > dataBegin) {
                    return it - v.it;
                } else {
                    if (it >= dataBegin && v.it >= dataBegin) {
                        return it - v.it;
                    } else if (it <= dataEnd && v.it <= dataEnd) {
                        return it - v.it;
                    } else if (it >= dataBegin && v.it <= dataEnd) {
                        return (memoryEnd - it) + (v.it - memoryBegin);
                    } else if (v.it >= dataBegin && it <= dataEnd) {
                        return (memoryEnd - v.it) + (it - memoryBegin);
                    }
                }
            }
            return 0;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator<(const normal_iterator &v) const {
            if (it == v.it) return false;
            if ((dataBegin > dataEnd) && ((it >= dataBegin && it < memoryEnd) || it < memoryBegin) &&
                ((v.it <= dataEnd && v.it >= memoryBegin) || v.it >= memoryEnd)) {
                return true;
            } else if ((dataBegin > dataEnd) && ((v.it >= dataBegin && v.it < memoryEnd) || v.it < memoryBegin) &&
                       ((it <= dataEnd && it >= memoryBegin) || it >= memoryEnd)) {
                return false;
            } else return it < v.it;
        }

        bool operator>(const normal_iterator &v) const {
            return !(*this < v);
        }

        bool operator>=(const normal_iterator &v) const {
            if (it == v.it) {
                return true;
            } else {
                return *this > v;
            }
        }

        bool operator<=(const normal_iterator &v) const {
            if (it == v.it) {
                return true;
            } else {
                return *this < v;
            }
        }

        reference operator*() const {
            return *it;
        }

        //Swappable
        void swap(const normal_iterator &v) {
            _iterator copy = v.it;
            v.it = it;
            it = copy;
            copy = dataBegin;
            dataBegin = v.dataBegin;
            v.dataBegin = copy;
            copy = dataEnd;
            dataEnd = v.dataEnd;
            v.dataEnd = copy;
            copy = memoryBegin;
            memoryBegin = v.memoryBegin;
            v.memoryBegin = copy;
            copy = memoryEnd;
            memoryEnd = v.memoryEnd;
            v.memoryEnd = copy;
        }
    };

    typedef typename Alloc::value_type value_type;

    typedef typename Alloc::reference reference;

    typedef typename Alloc::const_reference const_reference;

    typedef typename Alloc::pointer pointer;

    typedef typename Alloc::const_pointer const_pointer;

    typedef typename Alloc::difference_type difference_type;

    typedef typename Alloc::size_type size_type;

    typedef normal_iterator<pointer> iterator;

    typedef normal_iterator<const_pointer> const_iterator;

    //Defining a non-member swap in the enclosing namespace,
    //which may forward to a member swap if access to non-public data members is required
    void swap(iterator &v1, iterator &v2) {
        v1.swap(v2);
    }

    //DefaultConstructible
    explicit CCircularBuffer(Alloc alloc = Alloc()) {
        bufferSize = 0;
        bufferMaxSize = 0;
        data = nullptr;
        beginIndex = 0;
        endIndex = 0;
    }

    //CopyConstructible
    CCircularBuffer(const CCircularBuffer &c, Alloc alloc = Alloc()) {
        data = alloc.allocate(c.bufferMaxSize + 1);
        for (size_type i = 0; i < c.bufferSize; ++i) alloc.construct(&data[i]);
        bufferSize = c.bufferSize;
        bufferMaxSize = c.bufferMaxSize;

        for (size_type i = 0; i < c.bufferSize; ++i) data[i] = c.data[i];

        beginIndex = c.beginIndex;
        endIndex = c.endIndex;
    }

    explicit CCircularBuffer(size_type n, Alloc alloc = Alloc()) {
        data = alloc.allocate(n + 1);
        bufferSize = 0;
        bufferMaxSize = n;
        beginIndex = 0;
        endIndex = 0;
    }

    CCircularBuffer(size_type n, value_type t, Alloc alloc = Alloc()) {
        data = alloc.allocate(n + 1);
        for (size_type i = 0; i < n; ++i) alloc.construct(&data[i]);
        for (size_type i = 0; i < n; ++i) data[i] = t;
        bufferSize = n;
        bufferMaxSize = n;
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    CCircularBuffer(iterator i, iterator j, Alloc alloc = Alloc()) {
        if (i > j) throw std::length_error("The first normal_iterator is greater than the second");
        bufferSize = j - i;
        bufferMaxSize = bufferSize;
        data = alloc.allocate(bufferSize + 1);
        for (size_type k = 0; k < bufferSize; ++k) alloc.construct(&data[k]);
        for (size_type k = 0; k < bufferSize; ++k) data[k] = *(i + k);
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    CCircularBuffer(std::initializer_list<value_type> il, Alloc alloc = Alloc()) {
        bufferSize = il.size();
        bufferMaxSize = il.size();
        data = alloc.allocate(bufferSize + 1);
        for (size_type i = 0; i < bufferSize; ++i) alloc.construct(&data[i]);
        for (size_type i = 0; i < bufferSize; ++i) data[i] = *(il.begin() + i);
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    //Destructible
    ~CCircularBuffer() {
        std::_Destroy_n(data, bufferSize);
        Alloc().deallocate(data, bufferSize);
    }

    CCircularBuffer &operator=(CCircularBuffer c) {
        Alloc alloc;
        std::_Destroy_n(data, bufferSize);
        alloc.deallocate(data, bufferMaxSize + 1);
        data = alloc.allocate(c.bufferMaxSize + 1);
        for (size_type i = 0; i < c.bufferMaxSize; ++i) data[i] = c.data[i];
        bufferSize = c.bufferSize;
        bufferMaxSize = c.bufferMaxSize;
        beginIndex = c.beginIndex;
        endIndex = c.endIndex;
        return *this;
    }

    CCircularBuffer &operator=(const std::initializer_list<value_type> &il) {
        std::_Destroy_n(data, bufferMaxSize + 1);
        Alloc().deallocate(data, bufferMaxSize + 1);
        data = Alloc().allocate(il.size() + 1);
        bufferSize = il.size();
        bufferMaxSize = il.size();
        beginIndex = 0;
        endIndex = bufferSize - 1;
        for (size_type i = 0; i < bufferSize; ++i) data[i] = *(il.begin() + i);
        return *this;
    }

    reference operator[](size_type n) {
        if (n >= bufferSize) throw std::out_of_range("Reference to an object out of buffer memory");
        return data[(beginIndex + n) % bufferSize];
    }

    iterator begin() const {
        return iterator(data + beginIndex, data, data + bufferSize, data + beginIndex, data + endIndex);
    }

    iterator end() const {
        return iterator(data + bufferSize, data, data + bufferSize, data + beginIndex, data + endIndex);
    }

    const_iterator cbegin() const {
        return const_iterator(data + beginIndex, data, data + bufferSize, data + beginIndex, data + endIndex);
    }

    const_iterator cend() const {
        return const_iterator(data + bufferSize, data, data + bufferSize, data + beginIndex, data + endIndex);
    }

    //EqualityComparable
    bool operator==(const CCircularBuffer &c) {
        if (bufferSize != c.bufferSize || bufferMaxSize != c.bufferMaxSize) {
            return false;
        } else for (size_type i = 0; i < bufferSize; ++i) if (data[i] != c.data[i]) return false;
        return true;
    }

    bool operator!=(const CCircularBuffer &c) { return !(*this == c); }

    //Swappable
    void swap(CCircularBuffer &c) {
        pointer t = data;
        data = c.data;
        c.data = t;
        size_t copy = bufferSize;
        bufferSize = c.bufferSize;
        c.bufferSize = copy;
        copy = beginIndex;
        beginIndex = c.beginIndex;
        c.beginIndex = copy;
        copy = endIndex;
        endIndex = c.endIndex;
        c.endIndex = copy;
        copy = bufferMaxSize;
        bufferMaxSize = c.bufferMaxSize;
        c.bufferMaxSize = copy;
    }

    friend void swap(const CCircularBuffer &a, const CCircularBuffer &b) {
        pointer t = a.data;
        a.data = b.data;
        b.data = t;
        size_t copy = a.bufferSize;
        a.bufferSize = b.bufferSize;
        b.bufferSize = copy;
        copy = a.beginIndex;
        a.beginIndex = b.beginIndex;
        b.beginIndex = copy;
        copy = a.endIndex;
        a.endIndex = b.endIndex;
        b.endIndex = copy;
    }

    size_type size() { return bufferSize; }

    size_type max_size() { return std::numeric_limits<size_type>::max(); }

    bool empty() { return bufferSize == 0; }

    iterator insert(iterator p, value_type t) {
        if (bufferSize < bufferMaxSize) {
            ++bufferSize;
            ++endIndex;
            for (iterator i = end() - 1; i >= p; --i) {
                *i = *(i - 1);
            }
            *p = t;
            return iterator(p);
        } else {
            if (p == begin()) return p;
            for (iterator i = begin(); i < p - 1; ++i) *i = *(i + 1);
            *(p - 1) = t;
            return iterator(p - 1);
        }
    }

    iterator insert(iterator p, size_type n, value_type t) {
        for (size_type i = 0; i < n && i < bufferMaxSize; ++i) insert(p, t);
        return iterator(p - 1);
    }

    iterator insert(iterator p, iterator i, iterator j) {
        if (i > j) throw std::length_error("The first iterator is greater than the second");
        size_type n = j - i;
        if (n == 0) return p;
        n %= bufferMaxSize;
        for (size_type k = 0; k < n; ++k) insert(p, *(i + k));

        return iterator(p - 1);
    }

    iterator insert(iterator p, std::initializer_list<value_type> il) {
        if (il.size() == 0 || p == begin()) return p;
        for (size_type i = 0, j = 0; i < il.size() && j < bufferMaxSize; ++i, ++j) insert(p, *(il.begin() + i));

        return iterator(p - 1);
    }

    iterator erase(iterator q) {
        size_type newBeginIndex = beginIndex, newEndIndex = endIndex;
        if (q.it >= q.dataBegin && q.it <= q.dataEnd) {
            --newEndIndex;
        } else if (q.it <= q.dataEnd && q.it >= q.memoryBegin) {
            --newEndIndex;
            --newBeginIndex;
        }
        size_type n = q - begin();
        if (n > bufferSize) throw std::out_of_range("Pointing to an object out of buffer memory");

        Alloc alloc;
        pointer newData = alloc.allocate(bufferMaxSize--);
        for (size_type i = 0; i < bufferMaxSize; ++i) alloc.construct(&newData[i]);
        --bufferSize;

        for (size_type i = 0, j = 0; i < bufferSize + 1 && j < bufferSize; ++i, ++j) {
            if (data[i] == *q) {
                --j;
                continue;
            }
            newData[j] = data[i];
        }
        std::_Destroy_n(data, bufferMaxSize + 1);
        alloc.deallocate(data, bufferMaxSize + 1);
        data = newData;
        beginIndex = newBeginIndex;
        endIndex = newEndIndex;
        return bufferSize == 0 ? end() : iterator(begin() + n);
    }

    iterator erase(iterator q1, iterator q2) {
        size_type n = q2 - q1;
        if (n > bufferSize || q1.it < q1.memoryBegin || q2.it > q2.memoryEnd)
            throw std::out_of_range("Pointing to an object out of buffer memory");

        size_type newBeginIndex = beginIndex, newEndIndex = endIndex;
        if (q1.dataBegin < q1.dataEnd && q2.dataBegin < q2.dataEnd) {
            newEndIndex -= n;
        } else if (beginIndex > endIndex) {
            if (q1.it >= q1.dataBegin && q1.it < q1.memoryEnd && q2.it <= q2.dataEnd &&
                q2.it >= q2.memoryBegin) {
                newEndIndex -= q2.it - q2.memoryBegin;
                newBeginIndex -= q2.it - q2.memoryBegin;
            } else if (q2.it >= q2.dataBegin && q2.it < q2.memoryEnd && q1.it <= q1.dataEnd &&
                       q1.it >= q1.memoryBegin) {
                newEndIndex -= q1.it - q1.memoryBegin;
                newBeginIndex -= q1.it - q1.memoryBegin;
            } else if (q1.it >= q1.memoryBegin && q1.it <= q1.dataEnd && q2.it >= q2.memoryBegin &&
                       q2.it <= q2.dataEnd) {
                newEndIndex -= n;
                newEndIndex -= n;
            }
        }

        Alloc alloc;
        pointer newData = alloc.allocate(bufferMaxSize - n + 1);
        for (size_type i = 0; i < bufferMaxSize - n + 1; ++i) alloc.construct(&newData[i]);

        size_type m = 0;
        for (iterator it = begin(); it < end() && m < bufferSize - n; ++it, ++m) {
            if (it >= q1 && it < q2) {
                --m;
                continue;
            }
            newData[(newBeginIndex + m) % (bufferSize - n)] = *it;
        }

        bufferSize -= n;
        bufferMaxSize -= n;
        std::_Destroy_n(data, bufferMaxSize + n + 1);
        alloc.deallocate(data, bufferMaxSize + n + 1);
        data = newData;
        beginIndex = newBeginIndex;
        endIndex = newEndIndex;

        return bufferSize == 0 ? end() : iterator(begin());
    }

    void clear() {
        Alloc alloc;
        std::_Destroy_n(data, bufferSize);
        bufferSize = 0;
        endIndex = 0;
    }

    void assign(iterator i, iterator j) {
        if (i > j) throw std::length_error("The first iterator is greater than the second");
        size_type n = j - i;
        Alloc alloc;
        std::_Destroy_n(data, bufferMaxSize + 1);
        alloc.deallocate(data, bufferMaxSize + 1);
        data = alloc.allocate(n + 1);
        for (size_type k = 0; k < n; ++k) alloc.construct(&data[k]);
        for (size_type l = 0; i < j; i++, l++) data[l] = *i;
        bufferSize = n;
        bufferMaxSize = n;
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    void assign(std::initializer_list<value_type> il) {
        auto i = il.begin(), j = il.end();
        size_type n = j - i;
        Alloc alloc;
        std::_Destroy_n(data, bufferMaxSize + 1);
        alloc.deallocate(data, bufferMaxSize + 1);
        data = alloc.allocate(n + 1);
        for (size_type k = 0; k < n; ++k) alloc.construct(&data[k]);
        for (size_type l = 0; i < j; i++, l++) data[l] = *i;
        bufferSize = n;
        bufferMaxSize = n;
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    void assign(size_type n, value_type t) {
        Alloc alloc;
        std::_Destroy_n(data, bufferMaxSize + 1);
        alloc.deallocate(data, bufferMaxSize + 1);
        data = alloc.allocate(n + 1);
        for (size_type k = 0; k < n; ++k) alloc.construct(&data[k]);
        for (size_type k = 0; k < n; ++k) data[k] = t;
        bufferSize = n;
        bufferMaxSize = n;
        beginIndex = 0;
        endIndex = bufferSize - 1;
    }

    reference front() {
        return data[beginIndex];
    }

    reference back() {
        return data[endIndex];
    }

    void push_front(value_type t) {
        if (bufferSize < bufferMaxSize) {
            Alloc alloc;
            alloc.construct(&data[bufferSize]);

            for (size_type i = bufferSize + 1; i >= 1; --i) data[i] = data[i - 1];
            if (bufferSize > 0) ++endIndex;
            ++bufferSize;
            data[0] = t;
        } else {
            data[endIndex] = t;
            if (endIndex == 0) {
                endIndex = bufferSize - 1;
            } else --endIndex;
            if (beginIndex == 0) {
                beginIndex = bufferSize - 1;
            } else --beginIndex;
        }
    }

    void push_back(value_type t) {
        if (bufferSize < bufferMaxSize) {
            Alloc alloc;
            alloc.construct(&data[bufferSize]);
            data[bufferSize] = t;
            if (bufferSize > 0) ++endIndex;
            ++bufferSize;
        } else {
            data[beginIndex] = t;
            if (beginIndex == bufferSize - 1) {
                beginIndex = 0;
            } else ++beginIndex;
            if (endIndex == bufferSize - 1) {
                endIndex = 0;
            } else ++endIndex;
        }
    }

    void pop_front() {
        erase(begin());
    }

    void pop_back() {
        erase(end() - 1);
    }

    size_type capacity() { return bufferMaxSize + 1; }

    void reserve(size_type n) {
        if (n > bufferMaxSize) {
            Alloc alloc;
            pointer newData = alloc.allocate(n + 1);

            size_type i = 0;
            for (auto it = begin(); it < end(), i < n; ++it, ++i) {
                alloc.construct(&newData[i]);
                newData[i] = *it;
            }

            std::_Destroy_n(data, bufferMaxSize + 1);
            alloc.deallocate(data, bufferMaxSize + 1);
            data = newData;
            beginIndex = 0;
            endIndex = bufferSize - 1;
            bufferMaxSize = n;
        }
    }

    void resize(size_type n, value_type t = value_type()) {
        if (n < bufferSize) {
            Alloc alloc;
            auto it = end() - 1;
            for (size_type i = 0; i < bufferSize - n; ++i, --it) alloc.destroy(&*it);
            bufferSize = n;
            endIndex = bufferSize - 1;
        } else if (n > bufferSize) {
            if (n > bufferMaxSize) {
                Alloc alloc;
                pointer newData = alloc.allocate(n + 1);
                for (size_type i = 0; i < n + 1; ++i) alloc.construct(&newData[i]);

                size_type i = 0;
                for (auto it = begin(); it < end(), i < n; ++it, ++i) newData[i] = *it;

                std::_Destroy_n(data, bufferMaxSize + 1);
                alloc.deallocate(data, bufferMaxSize + 1);
                data = newData;
                beginIndex = 0;
                endIndex = bufferSize - 1;
                bufferMaxSize = n;
                bufferSize = n;
            } else {
                Alloc alloc;
                for (size_type i = 0; i < n - bufferSize; ++i) alloc.construct(&data[++endIndex]);
                bufferSize += n - bufferSize;
            }
        }
    }
};

#endif
