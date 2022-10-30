
#include "Station.h"
#include "Utilities.h"
#include <iomanip>
#include <iostream>
using namespace std;

namespace sdds
{
	size_t Station::m_widthField{ 0 };
	size_t Station::id_generator{ 0 };

	Station::Station(const string& str) {
		++id_generator;
		stationId = id_generator;

		Utilities u;
		bool more = true;
		size_t next_pos{ 0 };
		for (int i = 0; i < 4 && more; i++) {
			try {
				string token = u.extractToken(str, next_pos, more);
				if (i == 0) {
					itemName = token;
				}
				else if (i == 1) {
					size_t _serial = stoi(token);
					serial = _serial;
				}
				else if (i == 2) {
					size_t _nItems = stoi(token);
					nItems = _nItems;
				}
				else if (i == 3) {
					m_widthField = u.getFieldWidth(); //get max fieldwith
					description = token;
				}

			}
			catch (...) {
				cout << "   ERROR\n";

			}
		}

	}

	const std::string& Station::getItemName() const {
		return itemName;
	}

	size_t Station::getNextSerialNumber() {
		return serial++;
	}

	size_t Station::getQuantity() const {
		return nItems;
	}

	void Station::updateQuantity() {
		if (nItems > 0) {
			nItems--;
		}
	}

	void Station::display(std::ostream& os, bool full) const {
		os.width(3);  os << std::setfill('0') << std::right << stationId << " | ";
		os.width(m_widthField); os.fill(' '); os << std::left << itemName << "| ";
		os.width(6); os << std::setfill('0') << std::right << serial << " | ";
		if (!full) {
			os << endl;
		}
		else {
			os.width(4);  os.fill(' '); os << std::right << nItems << " | " << description << endl;
		}
	}



}
