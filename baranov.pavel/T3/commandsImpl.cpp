#include "commands.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <scopeGuard.hpp>
#include "polygon.hpp"

void baranov::area(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  ScopeGuard guard(out);
  std::map< std::string, std::function< double(const Polygon &) > > cmds;
  {
    using namespace std::placeholders;
    cmds["EVEN"] = std::bind(sumArea, 0.0, _1, isEven);
    cmds["ODD"] = std::bind(sumArea, 0.0, _1, isOdd);
    cmds["MEAN"] = std::bind(countMeanArea, 0.0, _1, shapes.size());
  }

  std::string cmd;
  in >> cmd;
  if (shapes.empty() && cmd == "MEAN")
  {
    throw std::logic_error("There are no shapes");
  }
  std::function< double(const Polygon &) > areaFunctor;
  try
  {
    areaFunctor = cmds.at(cmd);
  }
  catch (const std::out_of_range &)
  {
    using namespace std::placeholders;
    size_t numOfVertexes = std::stoull(cmd);
    if (numOfVertexes < 3)
    {
      throw std::logic_error("Invalid vertexes count");
    }
    std::function< bool(const Polygon &) > predicate = std::bind(isNumOfVertexes, _1, numOfVertexes);
    areaFunctor = std::bind(sumArea, 0.0, _1, predicate);
  }
  std::vector< double > areas;
  std::transform(shapes.cbegin(), shapes.cend(), std::back_inserter(areas), areaFunctor);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

void baranov::max(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  if (shapes.empty())
  {
    throw std::logic_error("There are no shapes");
  }
  ScopeGuard guard(out);
  std::map< std::string, std::function< void() > > cmds;
  {
    using namespace std::placeholders;
    cmds["AREA"] = std::bind(maxArea, std::ref(shapes), std::ref(out));
    cmds["VERTEXES"] = std::bind(maxVertexes, std::ref(shapes), std::ref(out));
  }
  std::string cmd;
  in >> cmd;
  out << std::fixed << std::setprecision(1);
  cmds.at(cmd)();
}

void baranov::min(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  if (shapes.empty())
  {
    throw std::logic_error("There are no shapes");
  }
  ScopeGuard guard(out);
  std::map< std::string, std::function< void() > > cmds;
  {
    using namespace std::placeholders;
    cmds["AREA"] = std::bind(minArea, std::ref(shapes), std::ref(out));
    cmds["VERTEXES"] = std::bind(minVertexes, std::ref(shapes), std::ref(out));
  }
  std::string cmd;
  in >> cmd;
  out << std::fixed << std::setprecision(1);
  cmds.at(cmd)();
}

void baranov::count(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< bool(const Polygon &) > > cmds;
  {
    using namespace std::placeholders;
    cmds["EVEN"] = std::bind(isEvenVertexesCount, _1);
    cmds["ODD"] = std::bind(isOddVertexesCount, _1);
  }
  std::function< double(const Polygon &) > countFunctor;
  std::string cmd;
  in >> cmd;
  if (cmds.find(cmd) != cmds.end())
  {
    countFunctor = cmds.at(cmd);
  }
  else
  {
    size_t numOfVertexes = std::stoull(cmd);
    if (numOfVertexes < 3)
    {
      throw std::logic_error("Invalid vertexes count");
    }
    using namespace std::placeholders;
    countFunctor = std::bind(isNumOfVertexes, _1, numOfVertexes);
  }
  out << std::count_if(shapes.cbegin(), shapes.cend(), countFunctor);
}

void baranov::rect(std::vector< Polygon > & shapes, std::istream &, std::ostream & out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isRectangle);
}

void baranov::inFrame(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("Invalid polygon to compare");
  }
  auto frameRect = std::accumulate(shapes.cbegin(), shapes.cend(), rect_t { { 0, 0 }, { 0, 0 } }, extendFrameRect);
  using namespace std::placeholders;
  auto isInFrameRect = std::bind(isPointInRect, _1, std::ref(frameRect));
  size_t count = std::count_if(polygon.points.cbegin(), polygon.points.cend(), isInFrameRect);
  if (count == polygon.points.size())
  {
    out << "<TRUE>";
  }
  else
  {
    out << "<FALSE>";
  }
}

void baranov::lessArea(std::vector< Polygon > & shapes, std::istream & in, std::ostream & out)
{
  Polygon polygon;
  in >> polygon;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("Invalid polygon to compare");
  }
  using namespace std::placeholders;
  auto predicate = std::bind(areaComparator, _1, std::ref(polygon));
  size_t result = std::count_if(shapes.cbegin(), shapes.cend(), predicate);
  out << result;
}

