#include "iguana/reflection.hpp"
#include <filesystem>
#include <stdexcept>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "iguana/json_reader.hpp"
#include "test_headers.h"
#include <iguana/json_util.hpp>
#include <iguana/json_writer.hpp>
#include <iostream>
#include <limits>
#include <filesystem>

TEST_CASE("test canada.json") {
  std::cout << std::filesystem::current_path().string() << "\n";
  {
    std::string test_str = R"({
    "type": "FeatureCollection",
    "features": [{
      "type": "Feature",
      "properties": {
        "name": "Canada"
      },
      "geometry": {
        "type": "Polygon",
        "coordinates": [
          [
            [-65.613616999999977,
              43.420273000000009
            ]
          ]
        ]
      }
      }]
    })";

    FeatureCollection p;
    iguana::from_json(p, test_str);
    std::cout << p.type << "\n";
  }

  FeatureCollection t;
  iguana::from_json_file(t, "../data/canada.json");
  std::cout << t.type << "\n";
}

TEST_CASE("test apache_builds.json") {
  apache_builds t;
  iguana::from_json_file(t, "../data/apache_builds.json");
  std::cout << t.description << "\n";
  for (auto &v : t.views) {
    std::cout << v.name << ", " << v.url << "\n";
  }
}
TEST_CASE("test numbers.json")
{
  {
    std::string test_str = R"(
          [
           0.655561997649,
           0.54153630768
          ]
      )";

    std::vector<double> numbers;
    iguana::from_json(numbers, test_str);
    for (const auto& num : numbers)
    {
      std::cout << std::fixed << std::setprecision(12) << num << "\n";
    }
  }

  std::vector<double> numbers;
  iguana::from_json_file(numbers, "../data/numbers.json");

  iguana::string_stream ss;
  const std::string jsonName = "temp.json";
  iguana::to_json(numbers, ss);
  std::ofstream writeSteam(jsonName, std::ios::out);
  writeSteam << ss;
  writeSteam.flush();
  writeSteam.close();
  
  std::vector<double> dummy;
  iguana::from_json_file(numbers, jsonName);

  CHECK(dummy.size() == numbers.size());
  for (auto i = 0; i < numbers.size(); i++)
  {
    CHECK(numbers[i] - dummy[i] < std::numeric_limits<double>::epsilon());
  }
    if (std::filesystem::exists(jsonName))
    std::filesystem::remove(jsonName);
}
// doctest comments
// 'function' : must be 'attribute' - see issue #182
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4007) int main(int argc, char **argv) {
  return doctest::Context(argc, argv).run();
}
DOCTEST_MSVC_SUPPRESS_WARNING_POP
