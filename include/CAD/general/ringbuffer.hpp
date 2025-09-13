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
            std::size_t index;
        public:
            RingBufferIterator(RingBuffer<T> *rb, std::size_t index):rb{rb},index{index}{}
            T operator*(){return this->rb->buff[this->index];};
            void operator++(){
                ++this->index;
                if(this->index >= this->rb->size){
                    this->index = 0;
                }
            };
            bool operator==(RingBufferIterator<T> &it){return this->index == it.index && this->rb == it.rb;};
            bool operator!=(RingBufferIterator<T> &it){return this->index != it.index || this->rb != it.rb;};
        };

        template <class T>
        class RingBuffer{
        friend RingBufferIterator<T>;
        private:
            std::size_t size;
            T *buff;
            // First element added
            std::size_t beginIndex;
            // Latest element added
            std::size_t endIndex;
            bool empty;
        public:
            RingBuffer(std::size_t size):size{size},buff{new T[this->size]},beginIndex{0},endIndex{0},empty{true}{};
            ~RingBuffer(){delete[] this->buff;};
            void resize(std::size_t newSize){
                T *temp = new T[newSize];

                if(!this->empty){
                    if(this->endIndex > this->beginIndex){
                        std::size_t start = this->beginIndex;
                        std::size_t count = this->endIndex - start;
                        if(count > newSize){
                            start += count-newSize;
                            count = this->endIndex - start;
                        }
                        memcpy(temp, &this->buff[start], count*sizeof(T));
                        this->endIndex = count;
                    }else{
                        std::size_t start = this->beginIndex;
                        std::size_t count1 = this->size - start;
                        std::size_t count2 = this->endIndex;
                        std::size_t totalCount = count1 + count2;
                        if(count1 >= newSize){
                            totalCount = newSize;
                            memcpy(temp, &this->buff[this->endIndex - newSize], totalCount * sizeof(T));
                        }else if(totalCount >= newSize){
                            memcpy(temp, &this->buff[this->beginIndex], (this->size-this->beginIndex)*sizeof(T));
                            memcpy(&temp[this->size-this->beginIndex], this->buff, this->endIndex*sizeof(T));
                        }
                        this->endIndex = totalCount;
                    }
                    this->beginIndex = 0;
                }

                this->size = newSize;
                delete[] this->buff;
                this->buff = temp;
            }
            void add(T &element){
                if(this->empty){
                    this->empty = false;
                    this->buff[0] = element;
                    this->endIndex++;
                }else{
                    this->buff[this->endIndex] = element;
                    this->endIndex = (this->endIndex+1)%this->size;
                    if(this->endIndex == this->beginIndex){
                        this->beginIndex = (this->beginIndex+1)%this->size;
                    }
                }
            };
            std::size_t calculateSize(){
                if(this->empty){
                    return 0;
                }else if(this->endIndex > this->beginIndex){
                    return this->endIndex - this->beginIndex;
                }else{
                    return this->size - this->beginIndex + this->endIndex;
                }
            };

            RingBufferIterator<T> begin(){return RingBufferIterator<T>{this,this->beginIndex};};
            RingBufferIterator<T> end(){return RingBufferIterator<T>{this,this->endIndex};};
        };
    }
}