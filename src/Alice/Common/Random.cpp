#include "Random.hpp"

namespace Alice
{

std::mt19937 Random::s_random_engine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;

} // namespace Alice
