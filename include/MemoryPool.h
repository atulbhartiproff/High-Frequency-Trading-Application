#pragma once
#include <vector>
#include <cstddef>
#include <memory>

template <typename T, std::size_t BlockSize = 4096>
class MemoryPool {
private:
    union Slot {
        T element;
        Slot* next;
    };
    
    typedef char* data_pointer;
    typedef Slot slot_type;
    typedef Slot* slot_pointer;
    
    slot_pointer current_block;
    slot_pointer current_slot;
    slot_pointer last_slot;
    slot_pointer free_slots;
    
    std::vector<data_pointer> allocated_blocks;
    
    std::size_t padPointer(data_pointer p, std::size_t align) const noexcept {
        std::uintptr_t result = reinterpret_cast<std::uintptr_t>(p);
        return ((align - result) % align);
    }
    
    void allocateBlock() {
        data_pointer new_block = new char[BlockSize];
        allocated_blocks.push_back(new_block);
        
        data_pointer body = new_block + sizeof(slot_pointer);
        std::size_t body_padding = padPointer(body, alignof(slot_type));
        current_block = reinterpret_cast<slot_pointer>(body + body_padding);
        current_slot = current_block;
        last_slot = reinterpret_cast<slot_pointer>(new_block + BlockSize - sizeof(slot_type) + 1);
    }
    
public:
    MemoryPool() noexcept {
        current_block = nullptr;
        current_slot = nullptr;
        last_slot = nullptr;
        free_slots = nullptr;
    }
    
    ~MemoryPool() noexcept {
        for (auto block : allocated_blocks) {
            delete[] block;
        }
    }
    
    template <typename U, typename... Args>
    T* allocate(Args&&... args) {
        if (free_slots != nullptr) {
            slot_pointer result = free_slots;
            free_slots = free_slots->next;
            return reinterpret_cast<T*>(new (result) T(std::forward<Args>(args)...));
        } else {
            if (current_slot >= last_slot) {
                allocateBlock();
            }
            return reinterpret_cast<T*>(new (current_slot++) T(std::forward<Args>(args)...));
        }
    }
    
    void deallocate(T* p) noexcept {
        if (p != nullptr) {
            p->~T();
            reinterpret_cast<slot_pointer>(p)->next = free_slots;
            free_slots = reinterpret_cast<slot_pointer>(p);
        }
    }
};
