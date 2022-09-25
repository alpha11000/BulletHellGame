#include <cstdlib>

namespace lgc {
	class RandomUtil {
	public:
		static int getRandomIndex(int levels, int  level, int size) {
			int r = rand();

			level = (level == 0) ? 1 : level;
			int fac = levels / level;
			fac = (fac == 0) ? 1 : fac;

			//int fac = (levels - level) / size;
			//fac = (fac == 0) ? 1 : fac;
			//fac = (fac < 0)? -fac : fac;

			for (int i = size - 1; i > 0; i--) {
				//printf("%d,   %d\n", r, i);
				//std::cout << i << " , " << level << " , " << levels << " , " << levels - level << std::endl;
				if (r % (i * fac) == 0) {
					return i;
				}
			}

			return 0;
		}
	};
}