#include "../include/knn.hpp"
#include <cmath>
#include <limits>
#include "stdint.h"
#include "data_handler.hpp"

knn::knn(int k)
{
    this->k = k;
    this->neighbors = new std::vector<data *>();
    this->training_data = new std::vector<data *>();
    this->test_data = new std::vector<data *>();
    this->validation_data = new std::vector<data *>();
}
knn::knn()
{
    this->k = 0;
    this->neighbors = new std::vector<data *>();
    this->training_data = new std::vector<data *>();
    this->test_data = new std::vector<data *>();
    this->validation_data = new std::vector<data *>();
}
knn::~knn()
{
    delete this->neighbors;
    delete this->training_data;
    delete this->test_data;
    delete this->validation_data;
}

void knn::find_knearest(data *d)
{
    double min = std::numeric_limits<double>::max();
    double prev_min = min;
    int min_index = 0;
    for (int i = 0; i < k; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < training_data->size(); j++)
            {
                double dist = calculate_distance(d, training_data->at(j));
                training_data->at(j)->set_distance(dist);
                if (dist < min)
                {
                    min = dist;
                    min_index = j;
                }
            }
            neighbors->push_back(training_data->at(min_index));
            prev_min = min;
            min = std::numeric_limits<double>::max();
        }
        else
        {
            for (int j = 0; j < training_data->size(); j++)
            {
                double dist = training_data->at(j)->get_distance();
                training_data->at(j)->set_distance(dist);
                if (dist < min && dist > prev_min)
                {
                    min = dist;
                    min_index = j;
                }
            }
            neighbors->push_back(training_data->at(min_index));
            prev_min = min;
            min = std::numeric_limits<double>::max();
        }
    }
}
void knn::set_training_data(std::vector<data *> *p_training_data)
{
    this->training_data = p_training_data;
}
void knn::set_test_data(std::vector<data *> *p_test_data)
{
    this->test_data = p_test_data;
}
void knn::set_validation_data(std::vector<data *> *p_validation_data)
{
    this->validation_data = p_validation_data;
}
void knn::set_k(int p_k)
{
    this->k = p_k;
}

int knn::predict()
{
    std::map<uint8_t, int> class_freq;
    for (int i = 0; i < neighbors->size(); i++)
    {
        if (class_freq.find(neighbors->at(i)->get_label()) == class_freq.end())
        {
            class_freq[neighbors->at(i)->get_label()] = 1;
        }
        else
        {
            class_freq[neighbors->at(i)->get_label()]++;
        }
    }

    int best = 0;
    int max = 0;
    for (auto kv : class_freq)
    {
        if (kv.second > max)
        {
            max = kv.second;
            best = kv.first;
        }
    }
    neighbors->clear();
    return best;
}

double knn::calculate_distance(data *query_point, data *input)
{
    double distance = 0.0;
    if (query_point->get_feature_vector_size() != input->get_feature_vector_size())
    {
        printf("Error: Feature vector size mismatch.\n");
        return 0.0;
    }
#ifdef EUCLID
    for (unsigned i = 0; i < query_point->get_feature_vector_size(); i++)
    {
        distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
    }
    distance = sqrt(distance);
#elif defined MANHATTAN

#endif
    return distance;
}

double knn::validate_performance()
{
    double current_performance = 0.0;
    int count = 0;
    int data_index = 0;
    for (data *query_point : *validation_data)
    {
        find_knearest(query_point);
        int prediction = predict();
        // printf("%d -> %d\n", prediction, query_point->get_label());
        if (prediction == query_point->get_label())
        {
            count++;
        }
        data_index++;
        // printf("Current performance: %.3f %\n", ((double)count * 100.0) / (double)data_index);
    }
    current_performance = ((double)count * 100.0) / (double)validation_data->size();
    printf("Final performance for K = %d: %.3f %\n", k, current_performance);
    return current_performance;
}
double knn::test_performance()
{
    double current_performance = 0.0;
    int count = 0;
    for (data *query_point : *test_data)
    {
        find_knearest(query_point);
        int prediction = predict();
        if (prediction == query_point->get_label())
        {
            count++;
        }
    }
    current_performance = ((double)count * 100.0) / (double)test_data->size();
    printf("test performance: %.3f\n", current_performance);
    return current_performance;
}

int main()
{
    data_handler *dh = new data_handler();
    dh->read_feature_vector("../train-images-idx3-ubyte");
    dh->read_feature_labels("../train-labels-idx1-ubyte");
    dh->split_data();
    dh->count_classes();
    knn *knearest = new knn();
    knearest->set_training_data(dh->get_training_data());
    knearest->set_test_data(dh->get_test_data());
    knearest->set_validation_data(dh->get_validation_data());
    double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for (int i = 1; i <= 4; i++)
    {
        if (i == 1)
        {
            knearest->set_k(1);
            performance = knearest->validate_performance();
            best_performance = performance;
        }
        else
        {
            knearest->set_k(i);
            performance = knearest->validate_performance();
            if (performance > best_performance)
            {
                best_performance = performance;
                best_k = i;
            }
        }
    }
    knearest->set_k(best_k);
    knearest->test_performance();
    return 0;
}