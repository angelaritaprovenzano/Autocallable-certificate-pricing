#include <ql/quantlib.hpp>
#include <replicationpathpricer.hpp>


using namespace QuantLib;

// real constructor
ReplicationPathPricer::ReplicationPathPricer(Option::Type type,
											 Real strike,
											 boost::shared_ptr<YieldTermStructure> OISTermStructure,
											 Time maturity,
											 boost::shared_ptr<BlackVarianceSurface> varTS)
	: type_(type), strike_(strike), OISTermStructure_(OISTermStructure), maturity_(maturity), sigma_(varTS) {
	QL_REQUIRE(strike_ > 0.0, "strike must be positive");
	QL_REQUIRE(maturity_ > 0.0, "maturity must be positive");
}

/* The actual computation of the Profit&Loss for each single path.

In each scenario N rehedging trades spaced evenly in time over
the life of the option are carried out, using the Black-Scholes
hedge ratio.
*/
Real ReplicationPathPricer::operator()(const Path& path) const {

	Size n = path.length() - 1;
	QL_REQUIRE(n>0, "the path cannot be empty");

	// discrete hedging interval
	Time dt = maturity_ / n;

	// For simplicity, we assume the stock pays no dividends.
	Rate stockDividendYield = 0.0;

	// let's start
	Time t = 0;

	// stock value at t=0
	Real stock = path.front();

	// money account at t=0
	Real money_account = 0.0;

	/************************/
	/*** the initial deal ***/
	/************************/
	// option fair price (Black-Scholes) at t=0	

	DiscountFactor rDiscount = OISTermStructure_->discount(maturity_);
	DiscountFactor qDiscount = 1.00;
	Real forward = stock*qDiscount / rDiscount;
	Real stdDev = std::sqrt(sigma_->blackVariance(maturity_, strike_));
	boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(type_, strike_));
	BlackCalculator black(payoff, forward, stdDev, rDiscount);


	std::cout << "Option value: " << black.value() << std::endl
		<< "T: " << maturity_ << std::endl
		<< "Underlying value: " << stock << std::endl
		<< "Strike: " << strike_ << std::endl
		<< "Volatility: " << std::sqrt(sigma_->blackVariance(maturity_, strike_) / maturity_) << std::endl
		<< "Volatility_blackVol " << sigma_->blackVol(maturity_, strike_, true) << std::endl
		<< "Risk-Free Rate: " << OISTermStructure_->zeroRate(maturity_, Compounding::Continuous) << std::endl
		<< "Discount Factor: " << OISTermStructure_->discount(maturity_) << std::endl
		<< "Forward: " << forward << std::endl;


	// sell the option, cash in its premium
	money_account += black.value();
	// compute delta
	Real delta = black.delta(stock);
	// delta-hedge the option buying stock
	Real stockAmount = delta;
	money_account -= stockAmount*stock;

	/**********************************/
	/*** hedging during option life ***/
	/**********************************/
	for (Size step = 0; step < n - 1; step++) {

		// time flows
		t += dt;

		// accruing on the money account
		rDiscount = (OISTermStructure_->discount(maturity_)) / (OISTermStructure_->discount(t));
		money_account *= (1 / rDiscount);

		// stock growth:
		stock = path[step + 1];

		// recalculate option value at the current stock value,
		// and the current time to maturity

		forward = stock*qDiscount / rDiscount;
		BlackCalculator black(payoff, forward, std::sqrt(sigma_->blackForwardVariance(t, maturity_, strike_)), rDiscount);

		// recalculate delta
		delta = black.delta(stock);

		// re-hedging
		money_account -= (delta - stockAmount)*stock;
		stockAmount = delta;
	}

	/*************************/
	/*** option expiration ***/
	/*************************/
	// last accrual on my money account
	money_account *= (1 / rDiscount);
	//money_account *= std::exp(r_*dt);

	// last stock growth
	stock = path[n];

	// the hedger delivers the option payoff to the option holder
	Real optionPayoff = PlainVanillaPayoff(type_, strike_)(stock);
	money_account -= optionPayoff;

	// and unwinds the hedge selling his stock position
	money_account += stockAmount*stock;

	// final Profit&Loss
	return money_account;
}