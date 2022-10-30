
#include "Workstation.h"

using namespace std;

namespace sdds
{


	Workstation::Workstation(const std::string& str) : Station(str) {
	}

	Workstation::Workstation(const Workstation& other) {
		throw std::exception();

	}

	Workstation::Workstation(Workstation&& other) {
		throw std::exception();
	}



	void Workstation::fill(std::ostream& os) {
		//if there are orders and the front order is not filled
		if (m_orders.size() > 0 && !m_orders.front().isOrderFilled()) {
			//fill items of this station in the front order
			m_orders.front().fillItem(*this, os);


		}
	}

	bool Workstation::attemptToMoveOrder() {
		bool moved = false;
		
		if (m_orders.size() > 0 ) {
			//check if this station finished servicing items in the order
			bool service = m_orders.front().isItemFilled(Station::getItemName());

			//if this station finished service/can't service any items in the order or this station has no inventory
			if (service || Station::getQuantity() == 0) {
				//move it to next station
				if (m_pNextStation != nullptr) {
					(*m_pNextStation) += (std::move(m_orders.front())); //move to next station
					m_orders.pop_front(); //remove the moved object 
					moved = true;
				}
				else {
					//if there is no next station in the assembly line, then the order is moved into g_completed or g_incomplete queue
					if (m_orders.front().isOrderFilled()) {
						g_completed.push_back(std::move(m_orders.front()));
						m_orders.pop_front(); //remove the moved object 
						moved = true;
					}
					else if (!m_orders.front().isOrderFilled()) {
						g_incomplete.push_back(std::move(m_orders.front()));
						m_orders.pop_front(); //remove the moved object 
						moved = true;
					}
				}
			}
		}

		return moved;
	}

	void Workstation::setNextStation(Workstation* station) {
		m_pNextStation = station;
	}

	Workstation* Workstation::getNextStation() const {
		return m_pNextStation;
	}

	void Workstation::display(std::ostream& os) const {
		if (m_pNextStation != nullptr) {

			os << Station::getItemName() << " --> " << m_pNextStation->getItemName() << endl;
		}
		else { //this station is last workstation (no next station)
			os << Station::getItemName() << " --> End of Line" << endl;

		}
	}

	Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
		m_orders.push_back(std::move(newOrder));
		return *this;
	}

	size_t Workstation::nOrders()const {
		return m_orders.size();
	}

}
