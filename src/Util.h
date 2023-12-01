#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include <math.h>

enum class STR_CASE {
  NOCHANGE, UPPERCASE, LOWERCASE
};

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch, STR_CASE tCase = STR_CASE::NOCHANGE);
size_t tolowercase(std::string& txt);
size_t touppercase(std::string& txt);

#endif // UTILS