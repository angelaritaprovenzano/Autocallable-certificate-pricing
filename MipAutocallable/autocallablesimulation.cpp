#include <ql/quantlib.hpp>
#include <autocallablesimulation.hpp>
#include <autocallablepathpricer.hpp>

using namespace QuantLib;

Real repaymentValue(const Repayment& repayment,
	boost::shared_ptr<YieldTermStructure> riskFreeTermStructure,
	boost::shared_ptr<YieldTermStructure> riskyTermStructure);


AutocallableSimulation::AutocallableSimulation(boost::shared_ptr<Quote> underlying,	
	boost::shared_ptr<YieldTermStructure> qTermStructure,
	boost::shared_ptr<YieldTermStructure> bondTermStructure,
	boost::shared_ptr<YieldTermStructure> OISTermStructure,
	boost::shared_ptr<BlackVolTermStructure> volatility,
	Time maturity,
	Real strike,
	Date settlementDate)
	: underlying_(underlying), qTermStructure_(qTermStructure),	bondTermStructure_(bondTermStructure),
	OISTermStructure_(OISTermStructure), volatility_(volatility), maturity_(maturity), strike_(strike), settlementDate_(settlementDate){
}


void AutocallableSimulation::compute(Size nTimeSteps, Size nSamples, char modelType) {

	Real excerciselevel = 15.08;

	// EarlyRepaiments
	std::vector<Repayment> repayments;
	Repayment firstRepaiment = { 1000.00,
		0.0,
		0.0,
		std::vector<Date>{Date(21, February, 2018),
		Date(22, February, 2018),
		Date(23, February, 2018),
		Date(26, February, 2018),
		Date(27, February, 2018)},
		excerciselevel,
		Date(05, March, 2018) };
	repayments.push_back(firstRepaiment);

	Repayment secondRepaiment = { 1000.00,
		58.00,
		0.0,
		std::vector<Date>{Date(20, February, 2019),
		Date(21, February, 2019),
		Date(22, February, 2019),
		Date(25, February, 2019),
		Date(26, February, 2019)},
		excerciselevel,
		Date(04, March, 2019) };
	repayments.push_back(secondRepaiment);

	Repayment thirdRepaiment = { 1000.00,
		116.00,
		0.0,
		std::vector<Date>{Date(20, February, 2020),
		Date(21, February, 2020),
		Date(24, February, 2020),
		Date(25, February, 2020),
		Date(26, February, 2020)},
		excerciselevel,
		Date(04, March, 2020) };
	repayments.push_back(thirdRepaiment);

	Repayment maturityRepaiment = { 1000.00,
		174.00,
		0.0,
		std::vector<Date>{Date(23, February, 2021),
		Date(24, February, 2021),
		Date(25, February, 2021),
		Date(26, February, 2021),
		Date(01, March, 2021)},
		excerciselevel,
		Date(03, March, 2021) };
	repayments.push_back(maturityRepaiment);

	for (auto& r : repayments) {
		auto value = repaymentValue(r, OISTermStructure_, bondTermStructure_);
		r.value = value;
	}
	
	// The Monte Carlo model generates paths, according to the "diffusion process", 
	//using the PathGenerator
	// each path is priced using thePathPricer
	// prices will be accumulated into statisticsAccumulator
	Real Price = 0;
	Real MC_Error = 0;
	
	if (modelType == 'B') {
		std::cout << "\nCalcolo del prezzo con il modello di Black&Scholes...\n" << std::endl;

		//B&S model
		boost::shared_ptr<StochasticProcess> BSdiffusion(new BlackScholesMertonProcess(
			Handle<Quote>(underlying_),
			Handle<YieldTermStructure>(qTermStructure_),
			Handle<YieldTermStructure>(OISTermStructure_),
			Handle<BlackVolTermStructure>(volatility_)));

		const BigNatural seed = 1234;
		PseudoRandom::rsg_type rsg = PseudoRandom::make_sequence_generator(BSdiffusion->factors() * nTimeSteps, seed);

		typedef MultiVariate<PseudoRandom>::path_generator_type generator_type;
		boost::shared_ptr<generator_type> MyPathGenerator(new
			generator_type(BSdiffusion, TimeGrid(maturity_, nTimeSteps),
				rsg, false));

		boost::shared_ptr<PathPricer<MultiPath>> MyPathPricer(
			new AutocallablePathPricer(bondTermStructure_,
				OISTermStructure_,
				maturity_,
				strike_,
				settlementDate_,
				repayments));

		Statistics statisticsAccumulator;

		MonteCarloModel<MultiVariate, PseudoRandom>
			MCSimulation(MyPathGenerator,
				MyPathPricer,
				statisticsAccumulator,
				false);

		MCSimulation.addSamples(nSamples);

		Price = MCSimulation.sampleAccumulator().mean();
		MC_Error = MCSimulation.sampleAccumulator().errorEstimate();

	} 
	else if (modelType == 'H') {
		std::cout << "\nCalcolo del prezzo con il modello di Heston...\n" << std::endl;

		//Heston model
		Real v0 = 0.0292;
		Real kappa = 1.13;
		Real theta = 0.191;
		Real sigma = 0.74355254;
		Real rho = -0.58486121;
		Real epsilon = 0.718598576122673;
		theta *= (epsilon*epsilon);
		sigma *= epsilon;

		boost::shared_ptr<StochasticProcess> Hdiffusion(new HestonProcess(
			Handle<YieldTermStructure>(OISTermStructure_),
			Handle<YieldTermStructure>(qTermStructure_),
			Handle<Quote>(underlying_),
			v0, kappa, theta, sigma, rho));

		const BigNatural seed = 1234;
		PseudoRandom::rsg_type rsg = PseudoRandom::make_sequence_generator(Hdiffusion->factors() * nTimeSteps, seed);

		typedef MultiVariate<PseudoRandom>::path_generator_type generator_type;
		boost::shared_ptr<generator_type> MyPathGenerator(new
			generator_type(Hdiffusion, TimeGrid(maturity_, nTimeSteps),
				rsg, false));

		boost::shared_ptr<PathPricer<MultiPath>> MyPathPricer(
			new AutocallablePathPricer(bondTermStructure_,
				OISTermStructure_,
				maturity_,
				strike_,
				settlementDate_,
				repayments));

		Statistics statisticsAccumulator;

		MonteCarloModel<MultiVariate, PseudoRandom>
			MCSimulation(MyPathGenerator,
				MyPathPricer,
				statisticsAccumulator,
				false);

		MCSimulation.addSamples(nSamples);

		Price = MCSimulation.sampleAccumulator().mean();
		MC_Error = MCSimulation.sampleAccumulator().errorEstimate();
	}
	
	std::cout << " \nQuotazione = " << 1005.32 << std::endl;
	std::cout << " \nPrice = " << Price << std::endl;
	std::cout << " \nErrore = " << abs(1 - Price / 1005.32) * 100 << " % " << std::endl;
	std::cout << " \nErrore MC = " << MC_Error << std::endl;
}

Real repaymentValue(const Repayment& repayment,
	boost::shared_ptr<YieldTermStructure> riskFreeTermStructure,
	boost::shared_ptr<YieldTermStructure> riskyTermStructure) {
	boost::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(Handle<YieldTermStructure>(riskyTermStructure)));
	boost::shared_ptr<ZeroCouponBond> zc(new ZeroCouponBond(2, TARGET(), repayment.faceAmount, repayment.paymentDate));
	zc->setPricingEngine(bondEngine);
	Real zcValue = zc->NPV();
	Real couponValue = repayment.coupon * riskFreeTermStructure->discount(repayment.paymentDate);
	return zcValue + couponValue;
}
	
