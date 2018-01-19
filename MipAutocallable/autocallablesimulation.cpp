#include <ql/quantlib.hpp>
#include <autocallablesimulation.hpp>
#include <autocallablepathpricer.hpp>
#include <marketdata.hpp>

using namespace QuantLib;

Real repaymentValue(const Repayment& repayment,
	boost::shared_ptr<YieldTermStructure> riskFreeTermStructure,
	boost::shared_ptr<YieldTermStructure> riskyTermStructure);

Array calibrateHeston(boost::shared_ptr<YieldTermStructure>OISTermStructure,
	boost::shared_ptr<YieldTermStructure>qTermStructure,
	boost::shared_ptr<BlackVarianceSurface> varTS,
	boost::shared_ptr<Quote> underlying,
	const DayCounter dayCount,
	const Calendar calendar,
	Date settlementDate,
	Date expiryDate);

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
		Calendar calendar = TARGET();
		DayCounter dayCount = Actual365Fixed();
		auto varTS_ = MarketData::buildblackvariancesurface(settlementDate_, calendar);
		Array param = calibrateHeston(OISTermStructure_, qTermStructure_, varTS_, underlying_, dayCount, calendar, settlementDate_,Date(03, Mar, 2021));
				
		Real theta = param.at(0);
		std::cout << " \ntheta = " << theta << std::endl;
		Real kappa = param.at(1);
		std::cout << " \nkappa = " << kappa << std::endl;
		Real sigma = param.at(2);
		std::cout << " \nsigma = " << sigma << std::endl;
		Real rho = param.at(3);
		std::cout << " \nrho = " << rho << std::endl;
		Real v0 = param.at(4);
		std::cout << " \nv0 = " << v0 << std::endl;

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
	
	std::cout << " \nQuotazione = " << 973.55 << std::endl;
	std::cout << " \nPrice = " << Price << std::endl;
	std::cout << " \nErrore = " << abs(1 - Price / 973.55) * 100 << " % " << std::endl;
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
	
Array calibrateHeston(const boost::shared_ptr<YieldTermStructure>OISTermStructure,
	const boost::shared_ptr<YieldTermStructure>qTermStructure,
	const boost::shared_ptr<BlackVarianceSurface> varTS,
	boost::shared_ptr<Quote> underlying,
	const DayCounter dayCount,
	const Calendar calendar,
	Date settlementDate,
	Date expiryDate) {

	//initial guess
	const Real epsilon = 0.718598576122673;
	const Real v0 = 0.0292;
	const Real kappa = 1.13;
	const Real theta = 0.191 * (epsilon * epsilon);
	const Real sigma = 0.74355254 * epsilon;
	const Real rho = -0.58486121;

	boost::shared_ptr<HestonProcess> process(new HestonProcess(
		Handle<YieldTermStructure>(OISTermStructure),
		Handle<YieldTermStructure>(qTermStructure),
		Handle<Quote>(underlying),
		v0, kappa, theta, sigma, rho));
	boost::shared_ptr<HestonModel> model(boost::make_shared<HestonModel>(process));
	boost::shared_ptr<PricingEngine> engine(boost::make_shared<AnalyticHestonEngine>(model));
	std::vector<boost::shared_ptr<CalibrationHelper>> options;

	//expiry dates
	Date expiryDates[] = { settlementDate,
		Date(06, April, 2017),
		Date(07, April, 2017),
		Date(13, April, 2017),
		Date(20, April, 2017),
		Date(21, April, 2017),
		Date(27, April, 2017),
		Date(18, May, 2017),
		Date(19, May, 2017),
		Date(15, June, 2017),
		Date(16, June, 2017),
		Date(29, June, 2017),
		Date(14, September, 2017),
		Date(15, September, 2017),
		Date(14, December, 2017),
		Date(15, December, 2017),
		Date(15, March, 2018),
		Date(16, March, 2018),
		Date(14, June, 2018),
		Date(15, June, 2018),
		Date(20, December, 2018),
		Date(21, December, 2018),
		Date(20, June, 2019),
		Date(21, June, 2019),
		Date(19, December, 2019),
		Date(20, December, 2019),
		Date(17, December, 2020),
		Date(16, December, 2021),
		Date(31, December, 2021),
		Date(30, December, 2022) };
	std::vector<Date> dates(expiryDates, expiryDates + LENGTH(expiryDates));
	//strike prices for the vola-surface
	const Real K[] = { 14.00, 14.25, 14.50, 14.75, 15.00, 15.25, 15.50, 15.75, 16.00, 16.25, 16.50, 16.75, 17.00,
		17.25, 17.50, 17.75, 18.00, 18.50, 19.00, 20.00 };
	std::vector<Real> strikes(K, K + LENGTH(K));
	const Real s0 = 15.35;
	for (Size i = 0; i < strikes.size(); ++i) {
		for (Size j = 1; j < dates.size(); ++j) {
			const Period maturity((int)((dates[j] - settlementDate) / 7.), Weeks);
			boost::shared_ptr<Quote> vol(new SimpleQuote(varTS->blackVol(dates[j], strikes[i])));
			boost::shared_ptr<HestonModelHelper> helper(boost::make_shared<HestonModelHelper>(maturity, calendar,
				s0, K[i], Handle<Quote>(vol), Handle<YieldTermStructure>(OISTermStructure),
				Handle<YieldTermStructure>(qTermStructure), CalibrationHelper::ImpliedVolError));
			options.push_back(helper);
		}
	}
	for (Size i = 0; i < options.size(); ++i) {
		options[i]->setPricingEngine(engine);
	}
	LevenbergMarquardt om(1e-8, 1e-8, 1e-8);
	model->calibrate(options, om, EndCriteria(500, 40, 1.0e-8, 1.0e-8, 1.0e-8));
	Real tolerance = 3.0e-7;
	return model->params();
}
