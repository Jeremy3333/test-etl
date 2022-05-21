#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include "stdint.h"
#include "data.hpp"

class data_handler
{
private:
    std::vector<data *> *data_vector;  // all of data (pre-split)
    std::vector<data *> *train_vector; // train data
    std::vector<data *> *test_vector;  // test data
    std::vector<data *> *valid_vector; // valid data

    int num_classes;
    int feature_size;
    std::map<uint8_t, int> class_map;

    const double TRAIN_SET_RATIO = 0.75;
    const double TEST_SET_RATIO = 0.20;
    const double VALID_SET_RATIO = 0.05;

public:
    data_handler();
    ~data_handler();

    void read_feature_vector(std::string file_name);
    void read_feature_labels(std::string file_name);
    void split_data();
    void count_classes();

    uint32_t convert_to_little_endian(const unsigned char *bytes);
    uint32_t convert_to_high_endian(const unsigned char *bytes);

    std::vector<data *> *get_training_data();
    std::vector<data *> *get_test_data();
    std::vector<data *> *get_validation_data();
};

#endif