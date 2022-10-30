
#ifndef SDDS_WORKSTATION_H
#define SDDS_WORKSTATION_H
#include <iostream>
#include <string>
#include <deque>
#include "CustomerOrder.h"
#include "Station.h"

extern std::deque<sdds::CustomerOrder> g_pending;
extern std::deque<sdds::CustomerOrder> g_completed;
extern std::deque<sdds::CustomerOrder> g_incomplete;

namespace sdds
{


	class Workstation : public Station {
		std::deque<CustomerOrder> m_orders{};
		Workstation* m_pNextStation{ nullptr };

	public:
		Workstation() = default;
		Workstation(const std::string& str);
		Workstation(const Workstation& other);
		Workstation& operator=(const Workstation& src) = delete; //no copying allowed
		Workstation(Workstation&& other);
		Workstation& operator=(Workstation&& src) = delete; //no moving

		void fill(std::ostream& os);
		bool attemptToMoveOrder();
		void setNextStation(Workstation* station);
		Workstation* getNextStation() const;
		void display(std::ostream& os) const;
		Workstation& operator+=(CustomerOrder&& newOrder);
		size_t nOrders() const;
	};


}

#endif
