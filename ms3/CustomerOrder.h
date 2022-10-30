
#ifndef SDDS_CUSTOMERORDER_H
#define SDDS_CUSTOMERORDER_H
#include <string>
#include "Station.h"


namespace sdds
{
	struct Item
	{
		std::string m_itemName;
		size_t m_serialNumber{ 0 };
		bool m_isFilled{ false };

		Item(const std::string& src) : m_itemName(src) {};
	};

	class CustomerOrder {

		std::string m_name{};
		std::string m_product{};
		size_t m_cntItem{};
		Item** m_lstItem;
		static size_t m_widthField;
	public:
		CustomerOrder() = default;
		CustomerOrder(const std::string& str);
		CustomerOrder(const CustomerOrder& other);
		CustomerOrder& operator=(const CustomerOrder& src) = delete; //no copying allowed
		CustomerOrder(CustomerOrder&& other) noexcept;
		CustomerOrder& operator=(CustomerOrder&& src) noexcept;
		~CustomerOrder();
		bool isOrderFilled() const;
		bool isItemFilled(const std::string& itemName) const;
		void fillItem(Station& station, std::ostream& os);
		void display(std::ostream& os) const;


	};


}

#endif
