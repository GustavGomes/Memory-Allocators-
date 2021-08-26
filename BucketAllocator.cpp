#include <vector>
template <typename T>
class bucket_allocator {
public:
    typedef T value_type;

    explicit bucket_allocator(const size_t bucket_size = 1024): threshold_size_(bucket_size) {}

    ~bucket_allocator() = default;

    bucket_allocator(const bucket_allocator&) = delete;
    bucket_allocator& operator=(const bucket_allocator&) = delete;

    T* allocate(const size_t n) {
        for (size_t i = 0; i < buckets_.size(); i++) {
            if (buckets_[i].allocated_objs_ + n <= buckets_[i].bucket_size_) {
                T* new_address = buckets_[i].bucket_address_ + buckets_[i].allocated_objs_;
                buckets_[i].allocated_objs_ += n;
                return new_address;
            }
        }
        bucket new_bucket;
        new_bucket.bucket_size_ = (n > threshold_size_) ? n : threshold_size_;
        new_bucket.bucket_address_ =
                reinterpret_cast<T*>(new char[sizeof(T) * new_bucket.bucket_size_]);
        T* new_address = new_bucket.bucket_address_ + new_bucket.allocated_objs_;
        new_bucket.allocated_objs_ += n;
        buckets_.push_back(new_bucket);
        return new_address;
    }

    void deallocate(T* ptr, const size_t n) noexcept {
        for (size_t i = 0; i < buckets_.size(); i++) {
            if ((ptr >= buckets_[i].bucket_address_) &&
                (ptr <= buckets_[i].bucket_address_ + buckets_[i].bucket_size_)) {
                buckets_[i].deallocated_objs_ += n;
                if (buckets_[i].allocated_objs_ == buckets_[i].deallocated_objs_) {
                    delete[] reinterpret_cast<char*>(buckets_[i].bucket_address_);
                }
            }
        }
        for (size_t j = 0; j < buckets_.size(); j++) {
            if (buckets_[j].allocated_objs_ == buckets_[j].deallocated_objs_) {
                buckets_.erase(buckets_.begin() + j);
            }
        }
    }

private:
    size_t threshold_size_;

    struct bucket {
        T* bucket_address_;
        size_t bucket_size_;
        size_t allocated_objs_;
        size_t deallocated_objs_;

        bucket()
                : bucket_address_(nullptr), allocated_objs_(0), deallocated_objs_(0), bucket_size_(0) {}
    };

    std::vector<bucket> buckets_;
};
