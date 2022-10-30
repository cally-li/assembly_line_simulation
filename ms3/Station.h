
#ifndef SDDS_STATION_H
#define SDDS_STATION_H
#include <iostream>
#include <string>

namespace sdds
{

	class Station {
		size_t stationId{};
		std::string itemName{};
		std::string description{};
		size_t serial{};
		size_t nItems{};
		static size_t m_widthField;
		static size_t id_generator;
	public:
		Station() = default;
		Station(const std::string& str);
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};


}



#endif
