#include <iomanip>
#include <sstream>
#include <string>

namespace ez
{
template <typename T>
std::string
NumToString(const T& inNumber, const int inNumDecimalPlaces = 0, const bool inAddThousandsSeparators = false)
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(inNumDecimalPlaces) << inNumber;

  auto str = ss.str();
  if (str.empty())
    return str;

  if (!inAddThousandsSeparators)
    return str;

  auto len = str.length();
  auto negative = ((len && str[0] == '-') ? 1 : 0);
  auto dpos = str.find_last_of('.');
  auto dlen = 3 + (dpos == std::string::npos ? 0 : (len - dpos));
  if (dpos != std::string::npos && '.' != '.')
    str[dpos] = '.';

  while ((len - negative) > dlen)
  {
    str.insert(len - dlen, 1, ',');
    dlen += 4;
    ++len;
  }
  return str;
}
}