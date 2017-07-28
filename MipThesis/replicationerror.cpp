#include <iostream>
#include <ql/quantlib.hpp>
#include <replicationerror.hpp>
#include <replicationpathpricer.hpp>
#include <marketdata.h>

using namespace QuantLib;

ReplicationError::ReplicationError(Option::Type type,
								   Time maturity,
								   Real strike,
								   boost::shared_ptr<Quote> s0,
								   boost::shared_ptr<BlackVarianceSurface> varTS,
								   boost::shared_ptr<YieldTermStructure> OISTermStructure)
	: maturity_(maturity), payoff_(type, strike), strike_(strike), s0_(s0), sigma_(varTS), OISTermStructure_(OISTermStructure) {

	// value of the option
	DiscountFactor rDiscount = OISTermStructure_->discount(maturity_);
	DiscountFactor qDiscount = 1.0;
	Real forward = (s0_->value())*qDiscount / rDiscount;
	Real stdDev = std::sqrt(sigma_->blackVariance(maturity_, strike_));
	boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(payoff_));
	BlackCalculator black(payoff, forward, stdDev, rDiscount);
	std::cout << "Option value: " << black.value() << std::endl
		<< "T: " << maturity_ << std::endl
		<< "Underlying value: " << s0_->value() << std::endl
		<< "Strike: " << strike << std::endl
		<< "Volatility: " << std::sqrt(sigma_->blackVariance(maturity_, strike_) / maturity_) << std::endl
		<< "Volatility_blackVol " << sigma_->blackVol(maturity_, strike_, true) << std::endl
		<< "Risk-Free Rate: " << OISTermStructure_->zeroRate(maturity_, Compounding::Continuous) << std::endl
		<< "Discount Factor: " << OISTermStructure_->discount(maturity_) << std::endl
		<< "Forward: " << forward << std::endl;

	// store option's vega, since Derman and Kamal's formula needs it
	vega_ = black.vega(maturity_);

	std::cout << std::endl;

	std::cout << std::setw(8) << " " << " | "
		<< std::setw(8) << " " << " | "
		<< std::setw(8) << "P&L" << " | "
		<< std::setw(8) << "P&L" << " | "
		<< std::setw(12) << "Derman&Kamal" << " | "
		<< std::setw(8) << "P&L" << " | "
		<< std::setw(8) << "P&L" << std::endl;

	std::cout << std::setw(8) << "samples" << " | "
		<< std::setw(8) << "trades" << " | "
		<< std::setw(8) << "mean" << " | "
		<< std::setw(8) << "std.dev." << " | "
		<< std::setw(12) << "formula" << " | "
		<< std::setw(8) << "skewness" << " | "
		<< std::setw(8) << "kurtosis" << std::endl;

	std::cout << std::string(78, '-') << std::endl;

}


// The computation over nSamples paths of the P&L distribution
void ReplicationError::compute(Size nTimeSteps, Size nSamples)
{
	QL_REQUIRE(nTimeSteps>0, "the number of steps must be > 0");

	// hedging interval
	// Time tau = maturity_ / nTimeSteps;


	Calendar calendar = TARGET();
	DayCounter dayCount = Actual365Fixed();

	boost::shared_ptr<StochasticProcess1D> diffusion(new BlackScholesProcess(Handle<Quote>(s0_),
		Handle<YieldTermStructure>(OISTermStructure_),
		Handle<BlackVolTermStructure>(sigma_)));

	// Black Scholes equation rules the path generator:
	// at each step the log of the stock
	// will have drift and sigma^2 variance
	PseudoRandom::rsg_type rsg =
		PseudoRandom::make_sequence_generator(nTimeSteps, 0);

	bool brownianBridge = false;

	typedef SingleVariate<PseudoRandom>::path_generator_type generator_type;
	boost::shared_ptr<generator_type> myPathGenerator(new
		generator_type(diffusion, maturity_, nTimeSteps,
			rsg, brownianBridge));

	// The replication strategy's Profit&Loss is computed for each path
	// of the stock. The path pricer knows how to price a path using its
	// value() method

	auto pricersigma = MarketData::buildblackvariancesurface(Date(4, Apr, 2017), TARGET());  // Please fix me

	boost::shared_ptr<PathPricer<Path>> myPathPricer(
		new ReplicationPathPricer(payoff_.optionType(), strike_, OISTermStructure_, maturity_, pricersigma));


	// a statistics accumulator for the path-dependant Profit&Loss values
	Statistics statisticsAccumulator;

	// The Monte Carlo model generates paths using myPathGenerator
	// each path is priced using myPathPricer
	// prices will be accumulated into statisticsAccumulator
	MonteCarloModel<SingleVariate, PseudoRandom>
		MCSimulation(myPathGenerator,
			myPathPricer,
			statisticsAccumulator,
			false);

	// the model simulates nSamples paths
	MCSimulation.addSamples(nSamples);

	// the sampleAccumulator method
	// gives access to all the methods of statisticsAccumulator
	Real PLMean = MCSimulation.sampleAccumulator().mean();
	Real PLStDev = MCSimulation.sampleAccumulator().standardDeviation();
	Real PLSkew = MCSimulation.sampleAccumulator().skewness();
	Real PLKurt = MCSimulation.sampleAccumulator().kurtosis();

	// Derman and Kamal's formula
	Real theorStD = std::sqrt(M_PI / 4 / nTimeSteps)*vega_*std::sqrt(sigma_->blackVariance(maturity_, strike_) / maturity_);

	std::cout << std::fixed
		<< std::setw(8) << nSamples << " | "
		<< std::setw(8) << nTimeSteps << " | "
		<< std::setw(8) << std::setprecision(3) << PLMean << " | "
		<< std::setw(8) << std::setprecision(2) << PLStDev << " | "
		<< std::setw(12) << std::setprecision(2) << theorStD << " | "
		<< std::setw(8) << std::setprecision(2) << PLSkew << " | "
		<< std::setw(8) << std::setprecision(2) << PLKurt << std::endl;
}

