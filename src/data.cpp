#include "data.hpp"

/*
 *lien pour compiler (attention a bien mettre le chemin de votre sdl2 et sdl2_image et de le lancer dans le dossier du projet) :
 * Debug:
 * g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main
 *
 *Release
 * g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL2-w64/include  && g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/release/main
 */

data::data()
{
    feature_vector = new std::vector<uint8_t>();
    label = 0;
}
data::~data()
{
    delete feature_vector;
}

void data::set_feature_vector(std::vector<uint8_t> *p_feature_vector)
{
    feature_vector = p_feature_vector;
}
void data::append_to_feature_vector(uint8_t feature)
{
    feature_vector->push_back(feature);
}
void data::set_label(uint8_t p_label)
{
    label = p_label;
}
void data::set_enum_label(int p_enum_label)
{
    enum_label = p_enum_label;
}

int data::get_feature_vector_size()
{
    return feature_vector->size();
}
uint8_t data::get_label()
{
    return label;
}
uint8_t data::get_enum_label()
{
    return enum_label;
}

std::vector<uint8_t> *data::get_feature_vector()
{
    return feature_vector;
}