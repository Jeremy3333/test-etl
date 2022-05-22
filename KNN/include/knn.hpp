#ifndef __KNN_HPP__
#define __KNN_HPP__

#include <vector>
#include "data.hpp"

class knn
{
private:
    int k;
    std::vector<data *> *neighbors;
    std::vector<data *> *training_data;
    std::vector<data *> *test_data;
    std::vector<data *> *validation_data;

public:
    knn(int k);
    knn();
    ~knn();

    void find_knearest(data *d);
    void set_training_data(std::vector<data *> *p_training_data);
    void set_test_data(std::vector<data *> *p_test_data);
    void set_validation_data(std::vector<data *> *p_validation_data);
    void set_k(int p_k);

    int predict();
    double calculate_distance(data *query_point, data *input);
    double validate_performance();
    double test_performance();
};

#endif