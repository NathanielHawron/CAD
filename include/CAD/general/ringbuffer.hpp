#pragma once

#include <cstring>
#include <cmath>
#include <iostream>

namespace CAD{
    namespace general{
        template <class T>
        class RingBuffer;

        template <class T>
        struct RingBufferIterator{
        private:
            RingBuffer<T> *rb;
            int index;
        public:
            RingBufferIterator(RingBuffer<T> *rb, int index):rb{rb},index{index}{}
            T &operator*(){return this->rb->buff[(this->rb->beginIndex+this->index)%this->rb->maxElements];};
            void operator++(){
                ++this->index;
            };
            void operator--(){
                --this->index;
            }
            void operator+=(int di){
                this->index += di;
            }
            void operator-=(int di){
                this->index -= di;
            }
            bool operator==(const RingBufferIterator<T> &it) const {return this->index == it.index && this->rb == it.rb;};
            bool operator!=(const RingBufferIterator<T> &it) const {return this->index != it.index || this->rb != it.rb;};
        };

        template <class T>
        class RingBuffer{
        friend RingBufferIterator<T>;
        private:
            std::size_t maxElements;
            T *buff;
            // First element added
            std::size_t beginIndex;
            // Number of filled elements
            std::size_t elements;
        public:
            RingBuffer(std::size_t size):maxElements{size},buff{new T[this->maxElements]},beginIndex{0},elements{0}{};
            ~RingBuffer(){delete[] this->buff;};
            void resize(std::size_t newSize){
                T *temp = new T[newSize];

                if(this->elements > 0){
                    std::size_t index = 0;
                    auto it = this->begin();
                    if(newSize < this->elements){
                        it += this->elements - newSize;
                    }
                    while(it != this->end()){
                        temp[index++] = *it;
                        ++it;
                    }
                    if(newSize < this->elements){
                        this->elements = newSize;
                    }
                }else{
                    // No copy necessary
                }
                this->beginIndex = 0;
                this->maxElements = newSize;
                delete[] this->buff;
                this->buff = temp;
            }
            void push(const T &element){
                if(this->elements == this->maxElements){
                    this->buff[this->beginIndex] = element;
                    this->beginIndex = (this->beginIndex+1)%this->maxElements;
                }else{
                    this->buff[(this->beginIndex+this->elements)%this->maxElements] = element;
                    ++this->elements;
                }
            };
            T pop(){
                if(this->elements > 0){
                    auto temp = this->beginIndex;
                    this->beginIndex = (this->beginIndex+1)%this->maxElements;
                    --this->elements;
                    return temp;
                }else{
                    return {};
                }
            }
            std::size_t size(){
                return this->elements;
            };

            RingBufferIterator<T> begin(){return RingBufferIterator<T>{this,0};};
            RingBufferIterator<T> end(){return RingBufferIterator<T>{this,this->elements};};

            RingBufferIterator<T> rbegin(){return RingBufferIterator<T>{this,this->elements-1};};
            RingBufferIterator<T> rend(){return RingBufferIterator<T>{this,-1};};
        };
    }
}