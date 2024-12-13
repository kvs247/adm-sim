#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T> void writeVector(std::vector<T> &v, const std::string &fpath)
{
  std::ofstream f(fpath, std::ios::binary | std::ios::out);
  if (!f)
  {
    throw std::runtime_error("Failed to open output file: " + fpath);
  }

  f.write(reinterpret_cast<char *>(v.data()), v.size() * sizeof(T));
  f.close();
}

template <typename T> std::vector<T> readVector(const std::string &fpath)
{
  std::ifstream f(fpath, std::ios::binary | std::ios::ate);
  if (!f)
  {
    throw std::runtime_error("Failed to open input file: " + fpath);
  }

  std::streamsize fsize = f.tellg();
  if (fsize % sizeof(T) != 0)
  {
    throw std::runtime_error("Error: " + fpath + " file size is not a multiple of " + std::to_string(sizeof(T)));
  }

  size_t n = fsize / sizeof(T);
  std::vector<T> v(n);
  f.seekg(0, std::ios::beg);
  if (!f.read(reinterpret_cast<char *>(v.data()), fsize))
  {
    throw std::runtime_error("Error reading input file: " + fpath);
  }
  f.close();

  return v;
}