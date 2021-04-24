#ifndef _GLOBAL_DEFINES_H_
#define _GLOBAL_DEFINES_H_

#include <vector>

#ifdef __APPLE__
    #define R_PATH "../resources/"
#else
    #define R_PATH "resources/"
#endif

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

#ifndef ABS
#define ABS(N) ((N<0)?(-N):(N))
#endif // !ABS

#define MOVE_FACTOR 20
#define DELAY_TIME 10

#endif // !_GLOBAL_DEFINES_H_
