#include "Util.h"
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch, STR_CASE tCase)
{
		std::string nTxt = "" + txt;
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();

		if (tCase == STR_CASE::UPPERCASE)
			touppercase(nTxt);
		else if (tCase == STR_CASE::LOWERCASE)
			tolowercase(nTxt);

    // Decompose statement
    while (pos != std::string::npos) {
        strs.push_back(nTxt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
		strs.push_back(nTxt.substr(initialPos, std::min(pos, nTxt.size()) - initialPos + 1));

    return strs.size();
}

size_t tolowercase(std::string& txt) {
	std::transform(txt.begin(), txt.end(), txt.begin(), [](unsigned char c){ return std::tolower(c); });
	return txt.size();
}

size_t touppercase(std::string& txt) {
	std::transform(txt.begin(), txt.end(), txt.begin(), [](unsigned char c){ return std::toupper(c); });
	return txt.size();
}