#pragma once

#ifndef replication_path_pricer_hpp
#define replication_path_pricer_hpp

#include <ql/quantlib.hpp>

using namespace QuantLib;

// The key for the MonteCarlo simulation is to have a PathPricer that
// implements a value(const Path& path) method.
// This method prices the portfolio for each Path of the random variable
class ReplicationPathPricer : public PathPricer<Path> {
	public:
		// real constructor
		ReplicationPathPricer(Option::Type type,
			Real strike,
			boost::shared_ptr<YieldTermStructure> OISTermStructure,
			Time maturity,
			boost::shared_ptr<BlackVarianceSurface> varTS);

		// The value() method encapsulates the pricing code
		Real operator()(const Path& path) const;

	private:
		Option::Type type_;
		Real strike_;
		boost::shared_ptr<YieldTermStructure> OISTermStructure_;
		Time maturity_;
		boost::shared_ptr<BlackVarianceSurface> sigma_;
};


#endif // !replication_path_pricer_h
