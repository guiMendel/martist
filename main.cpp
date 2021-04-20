#include "Martist.hpp"
#include <cassert>
#include <sstream>
int main() {
  constexpr std::size_t WIDTH = 320, HEIGHT = 240;
  std::uint8_t buf[WIDTH * HEIGHT * 3];
  Martist martist(buf, WIDTH, HEIGHT, 0, 0, 0);
  std::string spec("ys\nyxaxcac\nyx*yxsyyaaxc*c*ayysy*sac*\n");
  std::istringstream in(spec);
  in >> martist;
  assert(martist.redDepth() == 2);
  assert(martist.greenDepth() == 4);
  assert(martist.blueDepth() == 8);
  std::ostringstream out;
  out << martist;
  assert(out.str() == spec);
  return 0;
}