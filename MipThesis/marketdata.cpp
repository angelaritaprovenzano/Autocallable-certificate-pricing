#include <ql/quantlib.hpp>
#include <marketdata.hpp>

using namespace QuantLib;

boost::shared_ptr<YieldTermStructure> MarketData::builddiscountingcurve(Date settlementDate, Natural fixingDays) {

	// eonia swap
	Rate s1wQuote = -0.0036;
	Rate s2wQuote = -0.0036;
	Rate s1mQuote = -0.00359;
	Rate s2mQuote = -0.00359;
	Rate s3mQuote = -0.00359;
	Rate s4mQuote = -0.00358;
	Rate s5mQuote = -0.00346;
	Rate s6mQuote = -0.00355;
	Rate s7mQuote = -0.00353;
	Rate s8mQuote = -0.00348;
	Rate s9mQuote = -0.00344;
	Rate s10mQuote = -0.00341;
	Rate s11mQuote = -0.00336;
	Rate s1yQuote = -0.00331;
	Rate s1_5yQuote = -0.00297;
	Rate s2yQuote = -0.00253;
	Rate s2_5yQuote = -0.00211;
	Rate s3yQuote = -0.00145;
	Rate s3_5yQuote = -0.00037;
	Rate s5yQuote = -0.00082;
	Rate s6yQuote = -0.00216;

	/********************
	***    QUOTES    ***
	********************/

	// SimpleQuote stores a value which can be manually changed;
	// other Quote subclasses could read the value from a database
	// or some kind of data feed.


	// eonia swaps
	boost::shared_ptr<Quote> s1wRate(new SimpleQuote(s1wQuote));
	boost::shared_ptr<Quote> s2wRate(new SimpleQuote(s2wQuote));
	boost::shared_ptr<Quote> s1mRate(new SimpleQuote(s1mQuote));
	boost::shared_ptr<Quote> s2mRate(new SimpleQuote(s2mQuote));
	boost::shared_ptr<Quote> s3mRate(new SimpleQuote(s3mQuote));
	boost::shared_ptr<Quote> s4mRate(new SimpleQuote(s4mQuote));
	boost::shared_ptr<Quote> s5mRate(new SimpleQuote(s5mQuote));
	boost::shared_ptr<Quote> s6mRate(new SimpleQuote(s6mQuote));
	boost::shared_ptr<Quote> s7mRate(new SimpleQuote(s7mQuote));
	boost::shared_ptr<Quote> s8mRate(new SimpleQuote(s8mQuote));
	boost::shared_ptr<Quote> s9mRate(new SimpleQuote(s9mQuote));
	boost::shared_ptr<Quote> s10mRate(new SimpleQuote(s10mQuote));
	boost::shared_ptr<Quote> s11mRate(new SimpleQuote(s11mQuote));
	boost::shared_ptr<Quote> s1yRate(new SimpleQuote(s1yQuote));
	boost::shared_ptr<Quote> s1_5yRate(new SimpleQuote(s1_5yQuote));
	boost::shared_ptr<Quote> s2yRate(new SimpleQuote(s2yQuote));
	boost::shared_ptr<Quote> s2_5yRate(new SimpleQuote(s2_5yQuote));
	boost::shared_ptr<Quote> s3yRate(new SimpleQuote(s3yQuote));
	boost::shared_ptr<Quote> s3_5yRate(new SimpleQuote(s3_5yQuote));
	boost::shared_ptr<Quote> s5yRate(new SimpleQuote(s5yQuote));
	boost::shared_ptr<Quote> s6yRate(new SimpleQuote(s6yQuote));

	/*********************
	***  RATE HELPERS ***
	*********************/

	// RateHelpers are built from the above quotes together with
	// other instrument dependant infos.  Quotes are passed in
	// relinkable handles which could be relinked to some other
	// data source later.

	//OISRateHelpers
	boost::shared_ptr<OvernightIndex> OvernightIndex(new Eonia);

	boost::shared_ptr<RateHelper>s1w(new OISRateHelper(
		fixingDays,
		1 * Weeks,
		Handle<Quote>(s1wRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s2w(new OISRateHelper(
		fixingDays,
		2 * Weeks,
		Handle<Quote>(s2wRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s1m(new OISRateHelper(
		fixingDays,
		1 * Months,
		Handle<Quote>(s1mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s2m(new OISRateHelper(
		fixingDays,
		2 * Months,
		Handle<Quote>(s2mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s3m(new OISRateHelper(
		fixingDays,
		3 * Months,
		Handle<Quote>(s3mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s4m(new OISRateHelper(
		fixingDays,
		4 * Months,
		Handle<Quote>(s4mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s5m(new OISRateHelper(
		fixingDays,
		5 * Months,
		Handle<Quote>(s5mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s6m(new OISRateHelper(
		fixingDays,
		6 * Months,
		Handle<Quote>(s6mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s7m(new OISRateHelper(
		fixingDays,
		7 * Months,
		Handle<Quote>(s7mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s8m(new OISRateHelper(
		fixingDays,
		8 * Months,
		Handle<Quote>(s8mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s9m(new OISRateHelper(
		fixingDays,
		9 * Months,
		Handle<Quote>(s9mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s10m(new OISRateHelper(
		fixingDays,
		10 * Months,
		Handle<Quote>(s10mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s11m(new OISRateHelper(
		fixingDays,
		11 * Months,
		Handle<Quote>(s11mRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s1y(new OISRateHelper(
		fixingDays,
		1 * Years,
		Handle<Quote>(s1yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s1_5y(new OISRateHelper(
		fixingDays,
		18 * Months,
		Handle<Quote>(s1_5yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s2y(new OISRateHelper(
		fixingDays,
		2 * Years,
		Handle<Quote>(s2yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s2_5y(new OISRateHelper(
		fixingDays,
		30 * Months,
		Handle<Quote>(s2_5yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s3y(new OISRateHelper(
		fixingDays,
		3 * Years,
		Handle<Quote>(s3yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s3_5y(new OISRateHelper(
		fixingDays,
		42 * Months,
		Handle<Quote>(s3_5yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s5y(new OISRateHelper(
		fixingDays,
		5 * Years,
		Handle<Quote>(s5yRate),
		OvernightIndex));
	boost::shared_ptr<RateHelper>s6y(new OISRateHelper(
		fixingDays,
		6 * Years,
		Handle<Quote>(s6yRate),
		OvernightIndex));

	/*********************
	**  CURVE BUILDING **
	*********************/

	// Any DayCounter would be fine.
	// ActualActual::ISDA ensures that 30 years is 30.0
	DayCounter termStructureDayCounter =
		ActualActual(ActualActual::ISDA);

	// The OIS-discounting curve
	std::vector<boost::shared_ptr<RateHelper> > OISInstruments;
	OISInstruments.push_back(s1w);
	OISInstruments.push_back(s2w);
	OISInstruments.push_back(s1m);
	OISInstruments.push_back(s2m);
	OISInstruments.push_back(s3m);
	OISInstruments.push_back(s4m);
	OISInstruments.push_back(s5m);
	OISInstruments.push_back(s6m);
	OISInstruments.push_back(s7m);
	OISInstruments.push_back(s8m);
	OISInstruments.push_back(s9m);
	OISInstruments.push_back(s10m);
	OISInstruments.push_back(s11m);
	OISInstruments.push_back(s1y);
	OISInstruments.push_back(s1_5y);
	OISInstruments.push_back(s2y);
	OISInstruments.push_back(s2_5y);
	OISInstruments.push_back(s3y);
	OISInstruments.push_back(s3_5y);
	OISInstruments.push_back(s5y);
	OISInstruments.push_back(s6y);

	boost::shared_ptr<YieldTermStructure> OISTermStructure(
		new PiecewiseYieldCurve<Discount, LogLinear>(
			settlementDate, OISInstruments,
			termStructureDayCounter));

	return OISTermStructure;
}


boost::shared_ptr<BlackVarianceSurface> MarketData::buildblackvariancesurface(Date settlementDate, Calendar calendar) {

	DayCounter dc = Actual365Fixed();

	//dates

	Date expiry0(settlementDate);
	Date expiry1(06, April, 2017);
	Date expiry2(07, April, 2017);
	Date expiry3(13, April, 2017);
	Date expiry4(20, April, 2017);
	Date expiry5(21, April, 2017);
	Date expiry6(27, April, 2017);
	Date expiry7(18, May, 2017);
	Date expiry8(19, May, 2017);
	Date expiry9(15, June, 2017);
	Date expiry10(16, June, 2017);
	Date expiry11(29, June, 2017);
	Date expiry12(14, September, 2017);
	Date expiry13(15, September, 2017);
	Date expiry14(14, December, 2017);
	Date expiry15(15, December, 2017);
	Date expiry16(15, March, 2018);
	Date expiry17(16, March, 2018);
	Date expiry18(14, June, 2018);
	Date expiry19(15, June, 2018);
	Date expiry20(20, December, 2018);
	Date expiry21(21, December, 2018);
	Date expiry22(20, June, 2019);
	Date expiry23(21, June, 2019);
	Date expiry24(19, December, 2019);
	Date expiry25(20, December, 2019);
	Date expiry26(17, December, 2020);
	Date expiry27(16, December, 2021);
	Date expiry28(31, December, 2021);
	Date expiry29(30, December, 2022);

	std::vector<Date> dates;
	dates.push_back(expiry0);
	dates.push_back(expiry1);
	dates.push_back(expiry2);
	dates.push_back(expiry3);
	dates.push_back(expiry4);
	dates.push_back(expiry5);
	dates.push_back(expiry6);
	dates.push_back(expiry7);
	dates.push_back(expiry8);
	dates.push_back(expiry9);
	dates.push_back(expiry10);
	dates.push_back(expiry11);
	dates.push_back(expiry12);
	dates.push_back(expiry13);
	dates.push_back(expiry14);
	dates.push_back(expiry15);
	dates.push_back(expiry16);
	dates.push_back(expiry17);
	dates.push_back(expiry18);
	dates.push_back(expiry19);
	dates.push_back(expiry20);
	dates.push_back(expiry21);
	dates.push_back(expiry22);
	dates.push_back(expiry23);
	dates.push_back(expiry24);
	dates.push_back(expiry25);
	dates.push_back(expiry26);
	dates.push_back(expiry27);
	dates.push_back(expiry28);
	dates.push_back(expiry29);

	//strike prices for the vola-surface

	Real K[] = { 14.00, 14.25, 14.50, 14.75, 15.00, 15.25, 15.50, 15.75, 16.00, 16.25, 16.50, 16.75, 17.00,
		17.25, 17.50, 17.75, 18.00, 18.50, 19.00, 20.00 };

	std::vector<Real> strikes(K, K + LENGTH(K)); //è un oggetto...

												 //volatility surface construction

	Volatility v[] =
	{ 0.23840, 0.21910, 0.19870, 0.17790, 0.15990, 0.14340, 0.13260, 0.13320, 0.13700, 0.14240, 0.15140, 0.16200, 0.17150, 0.17950, 0.18600, 0.19150, 0.19610, 0.20340, 0.20920, 0.21660,
		0.23610, 0.21720, 0.19710, 0.17690, 0.15940, 0.14330, 0.13270, 0.13310, 0.13670, 0.14170, 0.15010, 0.16040, 0.16990, 0.17800, 0.18460, 0.19010, 0.19490, 0.20240, 0.20820, 0.21570,
		0.21980, 0.20370, 0.18760, 0.17320, 0.16040, 0.14820, 0.13940, 0.13760, 0.13980, 0.14260, 0.14620, 0.15170, 0.15890, 0.16660, 0.17370, 0.18000, 0.18540, 0.19430, 0.20120, 0.21120,
		0.21160, 0.19820, 0.18580, 0.17480, 0.16460, 0.15480, 0.14740, 0.14460, 0.14550, 0.14750, 0.14980, 0.15260, 0.15640, 0.16160, 0.16750, 0.17350, 0.17920, 0.18890, 0.19670, 0.20810,
		0.21000, 0.19690, 0.18490, 0.17420, 0.16430, 0.15480, 0.14750, 0.14460, 0.14530, 0.14730, 0.14950, 0.15210, 0.15570, 0.16040, 0.16610, 0.17200, 0.17760, 0.18740, 0.19530, 0.20690,
		0.20160, 0.19110, 0.18150, 0.17320, 0.16700, 0.16290, 0.16100, 0.16090, 0.16220, 0.16480, 0.16820, 0.17240, 0.17690, 0.18170, 0.18640, 0.19090, 0.19520, 0.20300, 0.20960, 0.22010,
		0.22460, 0.21580, 0.20800, 0.20140, 0.19620, 0.19240, 0.18990, 0.18870, 0.18870, 0.18980, 0.19190, 0.19470, 0.19820, 0.20190, 0.20570, 0.20950, 0.21300, 0.21920, 0.22440, 0.23230,
		0.22480, 0.21620, 0.20850, 0.20200, 0.19680, 0.19310, 0.19060, 0.18940, 0.18940, 0.19050, 0.19250, 0.19540, 0.19880, 0.20250, 0.20630, 0.21000, 0.21350, 0.21980, 0.22500, 0.23300,
		0.23890, 0.23270, 0.22710, 0.22230, 0.21810, 0.21470, 0.21210, 0.21020, 0.20890, 0.20820, 0.20820, 0.20870, 0.20970, 0.21100, 0.21280, 0.21480, 0.21690, 0.22150, 0.22600, 0.23400,
		0.23860, 0.23240, 0.22700, 0.22230, 0.21830, 0.21510, 0.21260, 0.21090, 0.20970, 0.20920, 0.20920, 0.20970, 0.21070, 0.21210, 0.21370, 0.21570, 0.21780, 0.22230, 0.22670, 0.23450,
		0.24160, 0.23580, 0.23040, 0.22560, 0.22140, 0.21780, 0.21480, 0.21240, 0.21050, 0.20920, 0.20840, 0.20810, 0.20820, 0.20870, 0.20960, 0.21080, 0.21230, 0.21580, 0.21960, 0.22700,
		0.24120, 0.23710, 0.23320, 0.22970, 0.22640, 0.22350, 0.22080, 0.21840, 0.21630, 0.21440, 0.21280, 0.21140, 0.21030, 0.20930, 0.20860, 0.20810, 0.20790, 0.20790, 0.20850, 0.21150,
		0.24120, 0.23710, 0.23330, 0.22970, 0.22650, 0.22350, 0.22090, 0.21850, 0.21640, 0.21450, 0.21290, 0.21150, 0.21030, 0.20940, 0.20870, 0.20820, 0.20790, 0.20790, 0.20850, 0.21140,
		0.24170, 0.23850, 0.23540, 0.23240, 0.22970, 0.22710, 0.22480, 0.22250, 0.22050, 0.21860, 0.21690, 0.21540, 0.21400, 0.21280, 0.21170, 0.21080, 0.21000, 0.20890, 0.20840, 0.20870,
		0.24170, 0.23850, 0.23540, 0.23250, 0.22980, 0.22720, 0.22480, 0.22260, 0.22060, 0.21870, 0.21700, 0.21540, 0.21400, 0.21280, 0.21170, 0.21080, 0.21010, 0.20890, 0.20840, 0.20870,
		0.24250, 0.23970, 0.23700, 0.23440, 0.23200, 0.22970, 0.22760, 0.22550, 0.22360, 0.22190, 0.22030, 0.21880, 0.21740, 0.21610, 0.21500, 0.21390, 0.21300, 0.21140, 0.21020, 0.20860,
		0.24250, 0.23970, 0.23700, 0.23440, 0.23200, 0.22970, 0.22750, 0.22550, 0.22360, 0.22190, 0.22030, 0.21880, 0.21740, 0.21610, 0.21500, 0.21400, 0.21300, 0.21140, 0.21020, 0.20860,
		0.24630, 0.24410, 0.24190, 0.23990, 0.23790, 0.23590, 0.23410, 0.23230, 0.23060, 0.22900, 0.22750, 0.22600, 0.22460, 0.22330, 0.22210, 0.22090, 0.21990, 0.21790, 0.21630, 0.21380,
		0.24620, 0.24400, 0.24180, 0.23980, 0.23780, 0.23590, 0.23410, 0.23230, 0.23060, 0.22900, 0.22750, 0.22600, 0.22460, 0.22330, 0.22210, 0.22090, 0.21980, 0.21790, 0.21630, 0.21380,
		0.24710, 0.24520, 0.24330, 0.24160, 0.23980, 0.23810, 0.23650, 0.23490, 0.23340, 0.23200, 0.23050, 0.22920, 0.22790, 0.22660, 0.22550, 0.22430, 0.22320, 0.22120, 0.21940, 0.21640,
		0.24710, 0.24520, 0.24340, 0.24160, 0.23980, 0.23810, 0.23650, 0.23490, 0.23340, 0.23200, 0.23060, 0.22920, 0.22790, 0.22670, 0.22550, 0.22430, 0.22320, 0.22120, 0.21940, 0.21640,
		0.24960, 0.24800, 0.24640, 0.24490, 0.24340, 0.24200, 0.24060, 0.23920, 0.23790, 0.23670, 0.23550, 0.23430, 0.23320, 0.23210, 0.23110, 0.23010, 0.22920, 0.22740, 0.22580, 0.22300,
		0.24960, 0.24800, 0.24640, 0.24490, 0.24340, 0.24200, 0.24060, 0.23930, 0.23800, 0.23670, 0.23550, 0.23440, 0.23320, 0.23220, 0.23110, 0.23010, 0.22920, 0.22740, 0.22580, 0.22300,
		0.25090, 0.24950, 0.24810, 0.24670, 0.24540, 0.24410, 0.24290, 0.24170, 0.24060, 0.23950, 0.23840, 0.23740, 0.23640, 0.23540, 0.23450, 0.23360, 0.23280, 0.23120, 0.22970, 0.22720,
		0.25090, 0.24950, 0.24810, 0.24670, 0.24540, 0.24420, 0.24290, 0.24170, 0.24060, 0.23950, 0.23840, 0.23740, 0.23640, 0.23550, 0.23450, 0.23370, 0.23280, 0.23120, 0.22970, 0.22720,
		0.25310, 0.25190, 0.25070, 0.24960, 0.24860, 0.24750, 0.24650, 0.24550, 0.24460, 0.24370, 0.24280, 0.24190, 0.24110, 0.24030, 0.23960, 0.23880, 0.23810, 0.23680, 0.23550, 0.23330,
		0.25420, 0.25320, 0.25220, 0.25130, 0.25040, 0.24950, 0.24860, 0.24780, 0.24700, 0.24620, 0.24540, 0.24470, 0.24400, 0.24330, 0.24260, 0.24200, 0.24130, 0.24010, 0.23900, 0.23710,
		0.25440, 0.25340, 0.25240, 0.25140, 0.25050, 0.24960, 0.24880, 0.24790, 0.24710, 0.24630, 0.24560, 0.24480, 0.24410, 0.24340, 0.24280, 0.24210, 0.24150, 0.24030, 0.23920, 0.23730,
		0.25490, 0.25400, 0.25320, 0.25230, 0.25150, 0.25070, 0.24990, 0.24920, 0.24850, 0.24780, 0.24710, 0.24640, 0.24580, 0.24520, 0.24460, 0.24400, 0.24350, 0.24240, 0.24140, 0.23960
	};

	Matrix blackVolMatrix(strikes.size(), dates.size() - 1);
	for (Size i = 0; i < strikes.size(); ++i)
		for (Size j = 1; j < dates.size(); ++j) {
			blackVolMatrix[i][j - 1] = v[i*(dates.size() - 1) + j - 1];
		}

	const boost::shared_ptr<BlackVarianceSurface> varTS(
		new BlackVarianceSurface(settlementDate, calendar,
			std::vector<Date>(dates.begin() + 1, dates.end()),
			strikes, blackVolMatrix,
			dc));

	// Change interpolator to bicubic splines

	varTS->enableExtrapolation(true);
	varTS->setInterpolation<Bicubic>();

	//Cubic myInterpolation(CubicInterpolation::BoundaryCondition rightCondition
	//= CubicInterpolation::FirstDerivative, CubicInterpolation::BoundaryCondition leftCondition
	//= CubicInterpolation::FirstDerivative);


	//SvarTS->setInterpolation<CubicInterpolation::BoundaryCondition::SecondDerivative>;
	//varTS->enableExtrapolation(false);

	return varTS;

}