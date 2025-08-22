#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

void tokenize(const std::string &input,
              std::unordered_map<std::string, size_t> &hash_table) {
  if (input.empty())
    return;
  std::istringstream stream(input);
  std::string token;
  while (stream >> token) {
    hash_table[token]++;
  }
}

int main(int argc, char *argv[]) {
  std::unordered_map<std::string, size_t> hash_table{};
  if (argc < 2)
    return 1;
  std::ifstream ifs(argv[1]);
  if (!ifs.is_open()) {
    std::cerr << "Error: Could not open file '" << argv[1] << "'\n";
    return 1;
  }

  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  ifs.close();
  auto start = std::chrono::high_resolution_clock::now();

  tokenize(content, hash_table);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(end - start);
  std::cout << "Token counts:\n";
  std::vector<std::pair<std::string, size_t>> sorted_tokens(hash_table.begin(),
                                                            hash_table.end());
  std::sort(sorted_tokens.begin(), sorted_tokens.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  std::cout << "\nTop 10 most frequent tokens:\n";
  for (int i = 0; i < std::min(10, (int)sorted_tokens.size()); i++) {
    std::cout << sorted_tokens[i].first << ": " << sorted_tokens[i].second
              << std::endl;
  }
  std::cout << std::fixed << std::setprecision(6);
  std::cout << "Time elapsed: " << duration.count() << std::endl;

  return 0;
}
