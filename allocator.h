#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <type_traits>

constexpr size_t MAX_ELEMS = 10;

template <class T>
struct custom_allocator {
    using value_type = T;

    custom_allocator(size_t size = MAX_ELEMS + 1)
        : pool_size(size * sizeof(T)), 
          allocated(0), 
          memory_pool(static_cast<uint8_t*>(::operator new(size * sizeof(T)))) {}

    T* allocate(size_t n) {
        size_t bytes_needed = n * sizeof(T);
        
        // переполнение пула.
        if (allocated + bytes_needed > pool_size) {
            throw std::bad_alloc();
        }

        std::cout << "Allocating " << n << " objects from pool\n";
        T* ptr = reinterpret_cast<T*>(memory_pool + allocated);
        allocated += bytes_needed;
        return ptr;
    }

    void deallocate(T* p, [[maybe_unused]] std::size_t n) noexcept {
        if (p == nullptr) return;

        //принадлежит ли указатель нашему пулу
        uint8_t* ptr = reinterpret_cast<uint8_t*>(p);
        if (ptr >= memory_pool && ptr < memory_pool + pool_size) {
            return;
        }
        
        ::operator delete(p);
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    template <typename U>
	struct rebind
	{
		using other = custom_allocator<U>;
	};

private:
    size_t pool_size;       // размер пула в байтах
    size_t allocated;       //количество выделенных байт
    uint8_t* memory_pool;   // указатель на пул памяти
};

template <class T, class Allocator = std::allocator<T>>
class custom_vector {
public:
    T* GetFirst() {
        return size > 0 ? data : nullptr;
    }

    T* GetNext(T* currentElem) {
        if (!currentElem || currentElem < data || currentElem >= data + size)
            return nullptr;
        return (currentElem + 1 < data + size) ? currentElem + 1 : nullptr;
    }

    custom_vector(const size_t& custom_size) 
    {
        // аллоцирую фиксированный сайз в конструкторе
        data = alloc.allocate(custom_size);
        capacity = custom_size;
    };

    ~custom_vector() {
        for (size_t i = 0; i < size; ++i) {
            alloc.destroy(data + i);
        }
        alloc.deallocate(data, capacity);
    }

    void push_back(const T &x)
    {
        // просто конструирую объекты на уже аллоцированной памяти
        alloc.construct(data + size, x);
        ++size;
    }

private:
    size_t size = 0;
    size_t capacity = 0;
    T* data = nullptr;
    Allocator alloc;
};