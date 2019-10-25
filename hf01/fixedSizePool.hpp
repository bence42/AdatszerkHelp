#pragma once
#include <exception>
#include <array>
#include <stdlib.h>

template<class T, unsigned N> //N a lefoglalando tomb merete, maximum meret
class FixedSizePool {
    using uint = unsigned int;
    T* myMem;
    T* lastFree;
    uint index;

public: //A return nullptr es a (void)variable csak a warning-ok kiiktatasa miatt vannak ott,
        //torold oket, amikor elkezded az adott fuggvenyt implementalni

    FixedSizePool() : myMem(new std::array<T, N>), lastFree(myMem->begin()), index(0) {}
    ~FixedSizePool(){
        delete myMem;
    }
    T* allocate() {
        T* temp = lastFree;
        ++index;
        lastFree = myMem->begin() + index;
        return temp;
    }

    T* allocate(unsigned n) {
        T* temp = allocate();
        index += n;
        lastFree = myMem->begin() + index;
        return temp;
    }

    void deallocate(T* p) {
        (void)p;
    }

    void deallocate(T* p, unsigned n) {
        (void)n;
        (void)p;
    }

    T* construct(const T value) {
        (void)value;
        return nullptr;
    }
};
