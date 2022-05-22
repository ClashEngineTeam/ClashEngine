#include "Random.hpp"
#include "../Random/random.hpp"
using __Random = effolkronium::random_static;

namespace ClashEngine
{
    EXPORT_FUNC_EX(uint) MinGetRandomValue(uint minimum, uint maximum)
    {
        return __Random::get(minimum, maximum);
    }

    uint Random::Range(uint minimum, uint maximum)
    {
        return MinGetRandomValue(minimum, maximum);
    }
}