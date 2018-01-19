#pragma once
#ifndef autocallable_simulation_hpp
#define autocallable_simulation_hpp

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

#include <ql/quantlib.hpp>

using namespace QuantLib;

/* The AutocallableSimulation class carries out Monte Carlo simulations to evaluate
the price of the Investment Certificate taking into accoutn the autocallable event over
different, randomly generated scenarios of future stock price evolution.
*/

class AutocallableSimulation{
public:
	AutocallableSimulation::AutocallableSimulation(boost::shared_ptr<Quote> underlying,
		boost::shared_ptr<YieldTermStructure> qTermStructure,
		boost::shared_ptr<YieldTermStructure> bondTermStructure,
		boost::shared_ptr<YieldTermStructure> OISTermStructure,
		boost::shared_ptr<BlackVolTermStructure> volatility,
		Time maturity,
		Real strike,
		Date settlementDate);

	// the actual price computation over the MC scenario
	void compute(Size nTimeSteps, Size nSamples, char modelType);

private:
	boost::shared_ptr<Quote> underlying_;
	boost::shared_ptr<YieldTermStructure> qTermStructure_;
	boost::shared_ptr<YieldTermStructure> bondTermStructure_;
	boost::shared_ptr<YieldTermStructure> OISTermStructure_;
	boost::shared_ptr<BlackVolTermStructure> volatility_;
	Time maturity_;
	Real strike_;
	Date settlementDate_;
};

struct Repayment {
	Real faceAmount;
	Real coupon;
	Real value;
	std::vector<Date> evaluationDates;
	Real exerciseLevel;
	Date paymentDate;
};

#endif