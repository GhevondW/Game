#ifndef _GLOBAL_DEFINES_H_
#define _GLOBAL_DEFINES_H_

#include <vector>

#define R_PATH "/Users/ghevondgevorgyan/Desktop/Game/resources/"
#define TILE_SIZE 87

template<class T>
using Matrix = std::vector<std::vector<T>>;

using IntMatrix = Matrix<int>;

#ifndef MIN
#define MIN(a, b) (((a)<(b))?(a):(b))
#endif /* MIN */
#ifndef MAX
#define MAX(a, b) (((a)>(b))?(a):(b))
#endif  /* MAX */

#endif // !_GLOBAL_DEFINES_H_
