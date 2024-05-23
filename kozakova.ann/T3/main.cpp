#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <iterator>
#include <functional>
#include "Polygon.hpp"
#include "Commands.hpp"

int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    std::string fname = argv[1];
    std::ifstream file(fname);
    std::vector< kozakova::Polygon > polygons;
    using inputIt = std::istream_iterator< kozakova::Polygon >;
    while (!file.eof())
    {
      std::copy(inputIt{ file }, inputIt{}, std::back_inserter(polygons));
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::map < std::string, std::function< void(std::istream&, std::ostream&) > > commands;
    {
      using namespace std::placeholders;
      commands["AREA"] = std::bind(kozakova::areaCmd, std::cref(polygons), _1, _2);
      commands["MAX"] = std::bind(kozakova::maxCmd, std::cref(polygons), _1, _2);
      commands["MIN"] = std::bind(kozakova::minCmd, std::cref(polygons), _1, _2);
      commands["COUNT"] = std::bind(kozakova::countCmd, std::cref(polygons), _1, _2);
      commands["RECTS"] = std::bind(kozakova::rectsCmd, std::cref(polygons), _2);
      commands["MAXSEQ"] = std::bind(kozakova::maxseqCmd, std::cref(polygons), _1, _2);
    }

    std::string cmd;
    while (std::cin >> cmd)
    {
      try
      {
        commands.at(cmd)(std::cin, std::cout);
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    file.close();
  }
  else
  {
    std::cerr << "not file" << "\n";
    return 1;
  }
  return 0;
}
