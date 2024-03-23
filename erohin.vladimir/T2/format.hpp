#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <iosfwd>
#include <string>

namespace erohin
{
  struct DoubleFormat
  {
    double & ref;
  };

  struct LongLongFormat
  {
    long long & ref;
  };

  struct StringFormat
  {
    std::string & ref;
  };

  struct LabelFormat
  {
    std::string expected;
  };

  std::istream & operator>>(std::istream & input, DoubleFormat && dest);
  std::istream & operator>>(std::istream & input, LongLongFormat && dest);
  std::istream & operator>>(std::istream & input, StringFormat && dest);
  std::istream & operator>>(std::istream & input, LabelFormat && dest);
}

#endif
