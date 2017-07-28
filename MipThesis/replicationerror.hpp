#pragma once

#ifndef replication_error_hpp
#define replication_error_hpp

#include <ql/quantlib.hpp>

using namespace QuantLib;

/* The ReplicationError class carries out Monte Carlo simulations to evaluate
the outcome (the replication error) of the discrete hedging strategy over
different, randomly generated scenarios of future stock price evolution.
*/

class ReplicationError {
	public:
		ReplicationError(Option::Type type,
			Time maturity,
			Real strike,
			boost::shared_ptr<Quote> s0,
			boost::shared_ptr<BlackVarianceSurface> varTS,
			boost::shared_ptr<YieldTermStructure> OISTermStructure);

		// the actual replication error computation
		void compute(Size nTimeSteps, Size nSamples);

	private:
		Time maturity_;
		PlainVanillaPayoff payoff_;
		Real strike_;
		boost::shared_ptr<Quote> s0_;
		boost::shared_ptr<BlackVarianceSurface> sigma_;
		boost::shared_ptr<YieldTermStructure> OISTermStructure_;
		Real vega_;
};


#endif // !replication_error_hpp

