
#include "Utilities.h"
#include <sstream>

using namespace std;

namespace sdds
{
	char Utilities::m_delimiter{ ' ' };


	void Utilities::setFieldWidth(size_t newWidth) {
		m_widthField = newWidth;
	}

	size_t Utilities::getFieldWidth() const {
		return m_widthField;
	}

	std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
		istringstream sStream(str);
		string token{};

		sStream.clear();
		sStream.seekg(next_pos);
		if (sStream.peek() == m_delimiter) {
			more = false;
			throw std::exception();
		}
		getline(sStream, token, m_delimiter);


		if (!sStream.fail()) {  //if getline did not fail
			//remove ws
			token = trim(token);
			//store max token length
			if (m_widthField < token.size()) {
				m_widthField = token.size();
			}
			//if at end of string (after consuming token), more=false
			if (sStream.peek() == EOF) {
				more = false;
			}
			else {
				next_pos = sStream.tellg();
				more = true;
			}
		}


		return token;

	}

	void Utilities::setDelimiter(char newDelimiter) {
		m_delimiter = newDelimiter;
	}

	char Utilities::getDelimiter() {
		return m_delimiter;
	}

	std::string trim(std::string& str) {
		std::string whitespaces = " \t\f\v\n\r";

		size_t start = str.find_first_not_of(whitespaces);
		std::string leftTrimmed = (start == std::string::npos) ? str : str.substr(start); //return string if no whitespace found on left side
		size_t end = leftTrimmed.find_last_not_of(whitespaces);
		std::string trimmed = (end == std::string::npos) ? leftTrimmed : leftTrimmed.substr(0, end + 1);
		return trimmed;
	}

}
