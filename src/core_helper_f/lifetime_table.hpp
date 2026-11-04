#pragma once
#include "matrix_exceptions.hpp"

namespace machine
{   
    class lifetime_table
    {   
        template<typename>
        friend class Matrix;

        template<typename>
        friend class assign_flag;

        inline static std::vector<long long> matrix_ids{0};
        inline static std::mutex mxt;

        inline static void no_data(size_t id)
        {   
            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in no_data()");
            }
            std::lock_guard<std::mutex> lock(mxt);
            matrix_ids[id] = -std::abs(matrix_ids[id]);
        }

        inline static void remove_id(size_t id)
        {   
            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in remove_id()");
            }
            std::lock_guard<std::mutex> lock(mxt);
            matrix_ids[id] = 0;
        }

        inline static size_t get_id(const long long gen = 1)
        {   
            std::lock_guard<std::mutex> lock(mxt);
            for(size_t i{} ; i<matrix_ids.size() ; i++)
            {
                if(matrix_ids[i] == 0){
                    matrix_ids[i] = gen;
                    return i;
                }
            }

            auto i_size = matrix_ids.size();
            matrix_ids.resize(i_size*2, 0);
            matrix_ids[i_size] = gen;
            return i_size;
        }

        inline static void new_generation(size_t id){
            std::lock_guard<std::mutex> lock(mxt);

            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in new_generation()");
            }
            if(matrix_ids[id]<0){
                matrix_ids[id] = std::abs(matrix_ids[id]);
            }
            matrix_ids[id] = matrix_ids[id]+1;
        };

        public:
            inline static bool check_id(size_t id)
            {   
                std::lock_guard<std::mutex> lock(mxt);
                if(id >= matrix_ids.size()){throw indexing_mismatch("id index too large in check_id()");}
                return matrix_ids[id]>0;
            }

            inline static long long get_gen(size_t id)
            {   
                std::lock_guard<std::mutex> lock(mxt);
                if(id >= matrix_ids.size()){throw indexing_mismatch("id index too large get_gen()");}
                return matrix_ids[id];
            }
    };
}
