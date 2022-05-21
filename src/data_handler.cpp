#include "data_handler.hpp"
#include "iostream"
#include <random>

data_handler::data_handler()
{
    data_vector = new std::vector<data *>();
    train_vector = new std::vector<data *>();
    test_vector = new std::vector<data *>();
    valid_vector = new std::vector<data *>();
}
data_handler::~data_handler()
{
    delete data_vector;
    delete train_vector;
    delete test_vector;
    delete valid_vector;
}

void data_handler::read_feature_vector(std::string file_name)
{
    uint32_t header[4]; // MAGIC NUMBER, NUMBER OF IMAGES, NUMBER OF ROWS, NUMBER OF COLUMNS
    unsigned char bytes[4];
    FILE *f = fopen(file_name.c_str(), "rb");
    if (f)
    {
        for (int i = 0; i < 4; i++)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
            else
            {
                printf("Error reading file.\n");
                exit(1);
            }
        }
        printf("Done getting Inpute File Header!\n");
        int image_size = header[2] * header[3];
        for (int i = 0; i < header[1]; i++)
        {
            data *d = new data();
            uint8_t element[1];
            for (int j = 0; j < image_size; j++)
            {
                if (fread(element, sizeof(element), 1, f))
                {
                    d->append_to_feature_vector(element[0]);
                }
                else
                {
                    printf("Error reading file.\n");
                    exit(1);
                }
            }
            data_vector->push_back(d);
        }
        printf("Successfully read and store %lu feature vectors \n", data_vector->size());
    }
    else
    {
        printf("Error opening file!\n");
        exit(1);
    }
}
void data_handler::read_feature_labels(std::string file_name)
{
    uint32_t header[2]; // MAGIC NUMBER, NUMBER OF IMAGES, NUMBER OF ROWS
    unsigned char bytes[4];
    FILE *f = fopen(file_name.c_str(), "r");
    if (f)
    {
        for (int i = 0; i < 2; i++)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
            else
            {
                printf("Error reading file.\n");
                exit(1);
            }
        }
        printf("Done getting Labbel File Header!\n");
        for (int i = 0; i < header[1]; i++)
        {
            uint8_t element[1];
            if (fread(element, sizeof(element), 1, f))
            {
                data_vector->at(i)->set_label(element[0]);
            }
            else
            {
                printf("Error reading file!\n");
                exit(1);
            }
        }
        printf("Successfully read and store label \n");
    }
    else
    {
        printf("Error opening file!\n");
        exit(1);
    }
}
void data_handler::split_data()
{
    std::unordered_set<int> used_indexes;
    std::random_device rd;
    int train_size = data_vector->size() * TRAIN_SET_RATIO;
    int test_size = data_vector->size() * TEST_SET_RATIO;
    int valid_size = data_vector->size() * VALID_SET_RATIO;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, data_vector->size() - 0.0);

    // Training Data
    for (int i = 0; i < train_size; i++)
    {
        int index = (int)dist(mt); // random index
        while (used_indexes.find(index) != used_indexes.end())
        {
            index = (int)dist(mt);
        }
        used_indexes.insert(index);
        train_vector->push_back(data_vector->at(index));
    }

    // Test Data
    for (int i = 0; i < test_size; i++)
    {
        int index = (int)dist(mt); // random index
        while (used_indexes.find(index) != used_indexes.end())
        {
            index = (int)dist(mt);
        }
        used_indexes.insert(index);
        test_vector->push_back(data_vector->at(index));
    }

    // Validation Data
    for (int i = 0; i < valid_size; i++)
    {
        int index = (int)dist(mt); // random index
        while (used_indexes.find(index) != used_indexes.end())
        {
            index = (int)dist(mt);
        }
        used_indexes.insert(index);
        valid_vector->push_back(data_vector->at(index));
    }

    printf("Training Data Size: %lu\n", train_vector->size());
    printf("Test Data Size: %lu\n", test_vector->size());
    printf("Validation Data Size: %lu\n", valid_vector->size());
}
void data_handler::count_classes()
{
    int count = 0;
    for (unsigned int i = 0; i < data_vector->size(); i++)
    {
        if (class_map.find(data_vector->at(i)->get_label()) == class_map.end())
        {
            class_map[data_vector->at(i)->get_label()] = count;
            data_vector->at(i)->set_enum_label(count);
            count++;
        }
    }
    num_classes = count;
    printf("Successfully Extracted %d Unique Classes\n", num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char *bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

uint32_t data_handler::convert_to_high_endian(const unsigned char *bytes)
{
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

std::vector<data *> *data_handler::get_training_data()
{
    return train_vector;
}
std::vector<data *> *data_handler::get_test_data()
{
    return test_vector;
}
std::vector<data *> *data_handler::get_validation_data()
{
    return valid_vector;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    data_handler *dh = new data_handler();
    dh->read_feature_vector("./train-images-idx3-ubyte");
    dh->read_feature_labels("./train-labels-idx1-ubyte");
    dh->split_data();
    dh->count_classes();
    return 0;
}