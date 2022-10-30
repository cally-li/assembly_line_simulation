
#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>

using namespace std;

std::deque<sdds::CustomerOrder> g_pending;
std::deque<sdds::CustomerOrder> g_completed;
std::deque<sdds::CustomerOrder> g_incomplete;


namespace sdds
{
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
		if (file.empty()) {
			throw string("ERROR: No filename provided.");
		}

		ifstream f(file);
		if (!f)
			throw string("Unable to open [") + file + "] file.";

		size_t i = 0;
		bool foundFirstStation = false;
		while (!f.eof())
		{
			try {
				string line;
				string delimiter = "|";

				getline(f, line);   
				size_t pos = line.find(delimiter);
				string stationName;
				auto predicate = [&](Workstation* station) { return station->getItemName() == stationName; };


				if (pos != string::npos) { //delimiter found 
					//add first token as a station
					stationName = line.substr(0, pos);
					auto result = std::find_if(stations.begin(), stations.end(), predicate);
					m_activeLine.push_back(*result);
					//set first Station if found
					if (foundFirstStation) {
						m_firstStation = (m_activeLine.back());
						foundFirstStation = false;
					}

					//set it's next station
					stationName = line.substr(pos + 1);
					auto goesTo = std::find_if(stations.begin(), stations.end(), predicate);
					m_activeLine.back()->setNextStation(*goesTo);
				}
				else { //1 station (end of assembly line)
					stationName = line;
					auto result = std::find_if(stations.begin(), stations.end(), predicate);
					//m_activeLine.back()->setNextStation(*result); //set previous station's next station
					m_activeLine.push_back(*result);
					m_activeLine.back()->setNextStation(nullptr); //set next station as null (end of line)
					foundFirstStation = true;
				}


				i++;
			}
			catch (...) {
				cout << "   ERROR during file reading\n";

			}

		}

		f.close();

		m_cntCustomerOrder = g_pending.size();
	}


	
	void LineManager::reorderStations() {

		Workstation* nextStation;
		for (size_t i = 0; i < m_activeLine.size(); i++) {
			if (i == 0) {
				std::swap(m_activeLine[i], m_firstStation);
			}
			else {
				std::swap(m_activeLine[i], nextStation);
			}
			nextStation = m_activeLine[i]->getNextStation();
		}
		m_firstStation = m_activeLine.front();


	}

	bool LineManager::run(std::ostream& os) {
		
		static int count = 0;
		count++;
		bool allOrdersFilled = false;
		os << "Line Manager Iteration: " << count << endl;

		

		if (g_pending.size() > 0) {

			(*m_firstStation) += std::move(g_pending.front());
			g_pending.pop_front();
		}

		//try to fill the order
		for (auto station : m_activeLine) {
			station->fill(os);
		}
		for (auto station : m_activeLine) {
			station->attemptToMoveOrder();
		}

		//check if any stations still have orders after first order is passed through assembly line
		if (count > 1) {
			size_t nOrders = 0;
			//count all orders remaining in asssembly line
			for (size_t i = 0; i < m_activeLine.size() && !allOrdersFilled; i++) {
				nOrders += m_activeLine[i]->nOrders();
			}

			if (nOrders == 0) {
				allOrdersFilled = true;
			}
		}
	
		return allOrdersFilled;

	}

	void LineManager::display(std::ostream& os) const {
		for (auto station : m_activeLine) {
			station->display(os);
		}
	}


}
