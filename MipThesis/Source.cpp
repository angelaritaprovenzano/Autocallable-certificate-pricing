#include <boost/timer.hpp>
#include <iostream>
#include <ql/quantlib.hpp>
#include <marketdata.hpp>
#include <replicationerror.hpp>

#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif

using namespace QuantLib;


// Compute Replication Error as in the Derman and Kamal's research note
int main(int, char*[]) {

	try {

		boost::timer timer;
		std::cout << std::endl;

		//date setting
		Calendar calendar = TARGET();
		DayCounter dayCount = Actual365Fixed();
		Date todaysDate(31, March, 2017);

		Integer fixingDays = 2;
		Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);
		// must be a business day
		settlementDate = calendar.adjust(settlementDate);
		Settings::instance().evaluationDate() = todaysDate;
		todaysDate = Settings::instance().evaluationDate();

		//option input-data		
		Date optionExpiryDate(03, June, 2020);
		Time maturity = dayCount.yearFraction(settlementDate, optionExpiryDate);
		Real strike = 18.81;

		boost::shared_ptr<Quote> underlying(new SimpleQuote(15.35));

		//discounting curve and volatility term structure
		auto OISTermStructure = MarketData::builddiscountingcurve(settlementDate, fixingDays);
		auto varTS = MarketData::buildblackvariancesurface(settlementDate, calendar);

		//declaration of the ReplicatonError class
		ReplicationError rp(Option::Call, maturity, strike, underlying, varTS, OISTermStructure);

		//initialization of the ReplicationError.compute() method
		Size scenarios = 50000;
		Size hedgesNum;

		hedgesNum = 21;
		rp.compute(hedgesNum, scenarios);

		hedgesNum = 84;
		rp.compute(hedgesNum, scenarios);

		double seconds = timer.elapsed();
		Integer hours = int(seconds / 3600);
		seconds -= hours * 3600;
		Integer minutes = int(seconds / 60);
		seconds -= minutes * 60;
		std::cout << " \nRun completed in ";
		if (hours > 0)
			std::cout << hours << " h ";
		if (hours > 0 || minutes > 0)
			std::cout << minutes << " m ";
		std::cout << std::fixed << std::setprecision(0)
			<< seconds << " s\n" << std::endl;

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "unknown error" << std::endl;
		return 1;
	}
}