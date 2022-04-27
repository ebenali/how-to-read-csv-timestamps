#include <cerrno> // the global 'errno' constant
#include <chrono> // std::chrono::duration, std::chrono::system_clock
#include <fstream> // imports std::ifstream class
#include <iostream>
#include <regex> // for iterate over CSV components with a loop; seprator character ',' defined by regex
#include <string>
#include <vector>

#include <date/date.h> // date::parse(&std_timepoint, format)
#include <fmt/format.h> // fmt::format()

int main(int argc, char **argv) {
  const std::string binary_name = argv[0];

  if (argc < 2)
    throw std::runtime_error{fmt::format("usage: {} <input.csv>", binary_name)};

  const auto *csv_pathname = argv[1];
  std::ifstream in_csv_file{csv_pathname};

  if (!in_csv_file) {
    throw std::runtime_error{
        fmt::format("cannot open input .csv file {} for reading; reason: {}",
                    csv_pathname, std::strerror(errno))};
  }

  // read first line
  std::string csv_header_line;
  if (!std::getline(in_csv_file, csv_header_line))
    throw std::runtime_error{"can't read CSV header line; file empty?"};

  const std::regex csv_delim_regex{","};
  std::vector<std::string> csv_header_field_names;
  std::copy(
      std::sregex_token_iterator(csv_header_line.begin(), csv_header_line.end(),
                                 csv_delim_regex, -1),
      std::sregex_token_iterator(), std::back_inserter(csv_header_field_names));

  std::cout << fmt::format("CSV filename={}; number of fields={}\n",
                           csv_pathname, csv_header_field_names.size());

  if (csv_header_field_names.size() == 0)
    throw std::runtime_error{
        fmt::format("strange: could not detect valid CSV header line fields; "
                    "first line empty?")};

  int field_number = 1;
  for (auto const &csv_field_name : csv_header_field_names) {
    std::cout << fmt::format("field number {}: name: {}\n", field_number++,
                             csv_field_name);
  }

  std::string csv_line;
  while (std::getline(in_csv_file, csv_line)) {
    auto csv_line_iterator = std::sregex_token_iterator(
        csv_line.begin(), csv_line.end(), csv_delim_regex, -1);
    auto /* in reality it will be an int */ csv_field_index = 1;
    while (csv_line_iterator != std::sregex_token_iterator{}) {
      if (csv_field_index++ == 4) {
        const std::string timestamp_string_value = csv_line_iterator->str();

        std::chrono::time_point<std::chrono::system_clock,
                                std::chrono::duration<double>>
            fractional_seconds;
        std::istringstream conversion_stream{csv_line_iterator->str()};
        conversion_stream >> date::parse("%F %T", fractional_seconds);

        double seconds_since_epoch =
            fractional_seconds.time_since_epoch().count();

        std::cout << fmt::format("got a CSV line; 4th field (timestamp) "
                                 "value='{}'; numeric value={}\n",
                                 csv_line_iterator->str(), seconds_since_epoch);
      }
      csv_line_iterator++;
    }
  }
}
