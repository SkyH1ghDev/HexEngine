#pragma once
#include <string>
#include <initializer_list>
#include <intsafe.h>

namespace StringUtils
{
	std::string HResultToString(HRESULT hr);

	std::wstring NarrowToWide(const std::string& narrow);
	std::wstring NarrowToWide(std::string_view narrow);

	std::string WideToNarrow(const std::wstring& wide);
	std::string WideToNarrow(std::wstring_view wide);

	std::string Trim(const std::string& str, const std::string& trimmed = " \t");

	template<typename... Args>
	inline std::string PathChain(Args... args) 
	{
		std::string result;
		bool first = true;
		(std::initializer_list<int>{ (result += (first ? "" : "\\") + args, first = false, 0)... });
		return result;
	}
}
