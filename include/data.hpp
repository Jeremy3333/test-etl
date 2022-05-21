#ifndef __DATA_H // Include guard.
#define __DATA_H // Include guard.

#include <vector>   // Include vector.
#include "stdint.h" // Include uint32_t.
#include "stdio.h"  // Include printf.

class data
{
private:
    std::vector<uint8_t> *feature_vector; // No class at end
    uint8_t label;
    int enum_label; // A -> 1, B -> 2

public:
    data();
    ~data();
    void set_feature_vector(std::vector<uint8_t> *p_feature_vector);
    void append_to_feature_vector(uint8_t feature);
    void set_label(uint8_t p_label);
    void set_enum_label(int p_enum_label);

    int get_feature_vector_size();
    uint8_t get_label();
    uint8_t get_enum_label();

    std::vector<uint8_t> *get_feature_vector();
};

#endif // __DATA_H  // Include guard.