#pragma once
#include<type_traits>
#include <stdexcept>
#include <utility>
#include <iostream>

//
//enums and statics
//
namespace machx
{
    enum class Operation
    {
        multiply,
        divide,
        add,
        substract,
        modulo,
        equal,
        power,
        root
    };
    enum axis
    {
        X, //0
        Y //1
    };
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
}

namespace machx
{
    enum class Condition
    {   
        is_true,
        is_false,
        under,
        over,
        over_or,
        under_or,
        is,
        is_not
    };
}
class Slice
{   
    public:
        long long begin{};
        long long end{};
        explicit Slice() : begin{0}, end{0}{}
        explicit Slice(const long long bgn) : begin{bgn}, end{bgn+1}{}
        Slice(const long long bgn, const long long ending) : begin{bgn}, end{ending}{} //DO MOT ADD ANY VALID CHECKING 0 = ALL COLS / ROWS
        Slice(const Slice &other) : begin{other.begin} , end{other.end}{}
        ~Slice() = default;

        Slice& operator=(const Slice &other){
            begin = other.begin;
            end = other.end;
            return *this;
        }

        friend Slice operator+(Slice old_slice, Slice new_slice)
        {
            
            new_slice.end = (new_slice.end <= 0)? old_slice.end+new_slice.end : new_slice.end;
            new_slice.begin = (new_slice.begin<0)? old_slice.end+new_slice.begin : new_slice.begin;
            return {new_slice};
        }
};


//
//using for ease
//
namespace std{

    using sl = Slice;
}

namespace machx
{
    template<typename T>
    class array
    {   

        //
        //INDEX using signed ints
        //bool treated in classic c++ style


        private:
            long long m_allocated{1};
            long long m_size{};
            T* arr{new T[1]};

        public:
            array(long long total = 0 , const T &val = T{})
            : m_allocated{total}
            , m_size{total}
            , arr{new T[(total<1)? 1: total]}
            {
                std::fill(arr, arr+m_size, val);
            }
            array(const array<T>& other)
            : m_allocated{other.m_allocated}
            , m_size{other.m_size}
            , arr{new T[other.m_allocated]}
            {
                auto* begin1 = arr;
                auto* end = arr+m_size;
                auto* begin2 = other.arr;
                while(begin1 != end){
                    *(begin1++) = *(begin2++);
                }
            }
            array(array<T>&& other)noexcept
            : m_allocated{other.m_allocated}
            , m_size{other.m_size}
            , arr{other.arr}
            {
                other.m_allocated = 1;
                other.m_size = 0;
                other.arr = new T[1];
                //other vars deleted automatically
            }

            ~array(){
                delete[] arr;
            }
            array<T>& operator=(array<T> other)
            {
                if(this == &other) return *this;

                m_allocated = other.m_allocated;
                m_size = other.m_size;
                std::swap(arr, other.arr);
                return *this;
            }

            array<T>& reserve(long long reserve_s){
                if(reserve_s<=m_allocated) return *this;

                T* new_arr {new T[reserve_s]};
                
                for(long long i{} ; i<m_size ; i++){
                    new_arr[i] = std::move(arr[i]);
                }

                m_allocated = reserve_s;
                delete[] arr;
                arr = new_arr;

                return *this;
            }

            array<T>& resize(long long new_size, const T &val = T{}){
                if(new_size < 0){
                    throw std::runtime_error("new size cannot be negative");
                }

                if(new_size>m_allocated){
                    reserve(new_size);
                }
                if(new_size>m_size){
                    std::fill(arr+m_size, arr+new_size, val);
                }

                m_size = new_size;
                return *this;

            } 
            
            void push_back(const T &item){
                if(m_allocated <= m_size){
                    this->reserve(1 + m_size*2);
                }
                arr[m_size] = item;
                m_size++;
            }

            void push_back(T &&item){
                if(m_allocated <= m_size){
                    this->reserve(1 + m_size*2);
                }
                arr[m_size] = std::move(item);
                m_size++;
            }

            void pop_back(){
                if(m_size <= 0){
                    return;
                }
                m_size--;
            }

            template<typename ...Args>
            void emplace_back(Args &&...arguments){
                if(m_allocated <= m_size){
                    this->reserve(1 + m_size*2);
                }
                arr[m_size] = {std::forward<Args>(arguments)...};
                m_size++;
            }

            T& top(){
                if(m_size<=0){
                    throw(std::runtime_error("stack does not have elements"));
                }
                return arr[m_size-1];
            }
            const T& top()const {
                if(m_size<=0){
                    throw(std::runtime_error("stack does not have elements"));
                }
                return arr[m_size-1];
            }

            T* begin(){
                return arr;
            }

            T* end(){
                return arr+m_size;
            }

            const T* begin()const{
                return arr;
            }

            const T* end()const{
                return arr+m_size;
            }

            T& operator[](long long index){ //no bounds check here or in const
                return arr[index];
            }

            const T& operator[](long long index)const{
                return arr[index];
            }

            long long size()const {return m_size;}
            long long allocated()const{return m_allocated;}
    };       
}
