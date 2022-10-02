#include <cstdlib>

namespace lgc {
	class RandomUtil {
	public:
		static int getRandomIndex(int levels, int  level, int size) {
			int r = rand();

			level = (level == 0) ? 1 : level;
			int fac = levels / level;

			for (int i = size - 1; i > 0; i--) {
				if (r % (i * fac) == 0) {
					return i;
				}
			}
			return 0;
		}

		static int getRandom(int min, int max){
			int r = rand();
			int n = r % (max - min);
			n += min;

			return n;
		}
	};

	class RandomGen {
	public:
		//static std::vector<int> generateNormalVector(int )
		// https://stackoverflow.com/questions/13091249/distribute-number-over-a-bell-curve
	};
}