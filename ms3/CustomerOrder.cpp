

#include "CustomerOrder.h"
#include "Utilities.h"
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

namespace sdds {

	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder(const std::string& str)
	{
		Utilities utilities;
		size_t next_pos = 0;
		size_t firstItem = 0;
		bool more = true;


		m_name = utilities.extractToken(str, next_pos, more);
		m_product = utilities.extractToken(str, next_pos, more);
		m_cntItem = 0;

		firstItem = next_pos;

		//count number of items in order
		while (more) {
			utilities.extractToken(str, next_pos, more);
			m_cntItem++;
		}

		m_lstItem = new Item * [m_cntItem];

		more = true;

		for (size_t i = 0; i < m_cntItem; i++) {
			m_lstItem[i] = new Item(utilities.extractToken(str, firstItem, more));
		}

		if (m_widthField < utilities.getFieldWidth()) {
			m_widthField = utilities.getFieldWidth();
		}
	}


	CustomerOrder::CustomerOrder(const CustomerOrder& other) {
		throw std::exception();
	}
	CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept : m_lstItem(nullptr) {
		*this = std::move(other);

	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
		if (this != &other) {
			if (m_lstItem) {
				for (unsigned int i = 0; i < m_cntItem; i++) {
					delete m_lstItem[i];
				}
			}

			delete[] m_lstItem;

			m_name = other.m_name;
			m_product = other.m_product;
			m_cntItem = other.m_cntItem;
			m_lstItem = other.m_lstItem;

			other.m_lstItem = nullptr;

		}
		return *this;
	}

	CustomerOrder::~CustomerOrder() {
		if (m_lstItem) {
			for (size_t i = 0; i < m_cntItem; i++) {
				delete m_lstItem[i];
			}
		}
		delete[] m_lstItem;
		m_lstItem = nullptr;
	}

	bool CustomerOrder::isOrderFilled() const {
		bool allItemsFilled = true;
		for (size_t i = 0; i < m_cntItem && allItemsFilled; i++) {
			if (!m_lstItem[i]->m_isFilled) {
				allItemsFilled = false;
			};
		}
		return allItemsFilled;

	}

	bool CustomerOrder::isItemFilled(const std::string& itemName) const {
		bool ItemsFilled = true;
		for (size_t i = 0; i < m_cntItem && ItemsFilled; i++) {
			if (m_lstItem[i]->m_itemName == itemName) {
				if (!m_lstItem[i]->m_isFilled) {
					ItemsFilled = false;
				};
			}
		}
		return ItemsFilled;
	}

	void CustomerOrder::fillItem(Station& station, std::ostream& os) {
		bool found = false;
		for (size_t i = 0; i < m_cntItem && !found; i++) {
			if (m_lstItem[i]->m_itemName == station.getItemName() && !m_lstItem[i]->m_isFilled) {
				//fill the item if station has stock
				if (station.getQuantity() > 0) {
					station.updateQuantity();
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[i]->m_isFilled = true;
					os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << endl;
				}
				else {
					
					string unable= "    Unable to fill " + m_name + ", " + m_product + " [" + station.getItemName() + "]\n" ;
					os << unable;
					if (m_lstItem[i+1]->m_itemName == station.getItemName()) {
						os << unable;
					}

				}
				found = true; 

			};
		}

	}

	void CustomerOrder::display(std::ostream& os) const {
		os << m_name << " - " << m_product << endl;
		for (size_t i = 0; i < m_cntItem; i++) {
			os << "["; os.width(6); os << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] ";
			os.width(m_widthField); os.fill(' '); os << std::left << m_lstItem[i]->m_itemName << "   - ";
			string status = m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED";
			os << status << endl;
		}


	}

}