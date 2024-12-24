#pragma once

#include <fstream>
#include <ostream>
#include <sstream>
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

std::string makeBytesWrittenString(size_t bytes)
{
  static const size_t KILOBYTE = 1024;
  static const size_t MEGABYTE = KILOBYTE * 1024;
  static const size_t GIGABYTE = MEGABYTE * 1024;
  static const size_t TERABYTE = GIGABYTE * 1024;

  const double dBytes = static_cast<double>(bytes);

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2);

  if (bytes < KILOBYTE)
  {
    oss << bytes << " bytes";
  }
  else if (bytes < MEGABYTE)
  {
    oss << dBytes / KILOBYTE << " KiB";
  }
  else if (bytes < GIGABYTE)
  {
    oss << dBytes / MEGABYTE << " MiB";
  }
  else if (bytes < TERABYTE)
  {
    oss << dBytes / GIGABYTE << " GiB";
  }
  else
  {
    oss << ">1 TiB";
  }

  return oss.str();
}

template <typename T>
void logDataWritten(const std::vector<T> &v, const std::string &path, std::ostream &os = std::cout)
{
  const auto bytesStr = makeBytesWrittenString(v.size() * sizeof(T));
  os << "wrote " << bytesStr << " to " << path << "\n";
};