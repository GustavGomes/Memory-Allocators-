#include <iostream>

template <typename T>
class linear_allocator {
public:
    typedef T value_type;

    explicit linear_allocator (const size_t buffer_size = k_buffer_size):
            _buffer_size(buffer_size),
            _buffer(reinterpret_cast<T*>(new (std::nothrow) char[sizeof(T) * k_buffer_size])),
            _allocations_count(0)
    {}

    ~linear_allocator () { delete[] reinterpret_cast<char*>(_buffer); };

    linear_allocator(const linear_allocator<T>) = delete;
    linear_allocator& operator=(const linear_allocator<T>) = delete;

    T* allocate (const size_t n) {
        if (_allocations_count + n > _buffer_size) {
            throw std::bad_alloc();
        }
        const size_t current_allocations_count = _allocations_count;
        _allocations_count += n;
        return _buffer + current_allocations_count;
    }

    void deallocate (T*, const size_t n) const noexcept {}

    size_t max_size () const noexcept {
        return _buffer_size;
    }

private:
    const static size_t k_buffer_size = 1024;
    size_t _buffer_size;
    T* _buffer;
    size_t _allocations_count;
};
