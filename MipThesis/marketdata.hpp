#pragma once

#ifndef market_data_hpp
#define market_data_hpp

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

#include <ql/quantlib.hpp>

using namespace QuantLib;


struct MarketData {

	static boost::shared_ptr<BlackVarianceSurface> buildblackvariancesurface(Date settlementDate,
																		     Calendar calendar);

	static boost::shared_ptr<YieldTermStructure> builddiscountingcurve(Date settlementDate,
																	   Natural fixingDays);

};

#endif // !market_data_hpp

