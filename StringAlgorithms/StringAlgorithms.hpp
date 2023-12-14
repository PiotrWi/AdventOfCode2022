#pragma once

#include <functional>
#include <string>
#include <span>

std::string ltrim(const std::string& in);
std::string rtrim(const std::string& in);
std::string trim(const std::string& in);

std::vector<std::string> splitAndTrim(const std::string& in, char c);
std::vector<std::string> splitAndTrimByStr(const std::string& in, std::string delimeter);

std::vector<std::string> splitNumbersAndLetters(const std::string& in);
std::vector<std::string> splitNumbersAndChars(const std::string& in);

std::vector <std::string> transpose(const std::vector <std::string>& input);

namespace s_view
{

std::vector < std::span<char> > splitGroups(std::string& in, std::function<bool(char)> groupPredicate);

}  // namespace s_view