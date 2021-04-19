#ifndef _GLOBAL_DEFINES_H_
#define _GLOBAL_DEFINES_H_

#include <vector>

#define R_PATH "..\\..\\resources\\"
#define TILE_SIZE 87

template<class T>
using Matrix = std::vector<std::vector<T>>;

using IntMatrix = Matrix<int>;

#endif // !_GLOBAL_DEFINES_H_
