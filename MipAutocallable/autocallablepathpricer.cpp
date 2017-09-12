#include <ql/quantlib.hpp>
#include <autocallablepathpricer.hpp>

using namespace QuantLib;

Repayment occurredRepayment(const std::vector<Repayment>& earlyRepaiments,
							const Path& stockPath,
							const DayCounter& dayCount,
							const Date& settlementDate);

Real stockValue(const Path& path, const Date& date,
				const DayCounter& dayCount, const Date& settlementDate);

Real computeAverage(const Repayment& r, const Path& stockPath,
					const DayCounter& dayCount,
					const Date& settlementDate);


// real constructor
AutocallablePathPricer::AutocallablePathPricer(boost::shared_ptr<YieldTermStructure> bondTermStructure,
	boost::shared_ptr<YieldTermStructure> OISTermStructure,
	Time maturity,
	Real strike,
	Date settlementDate,
	std::vector<Repayment> repayments)
	: bondTermStructure_(bondTermStructure), OISTermStructure_(OISTermStructure), maturity_(maturity), strike_(strike), settlementDate_(settlementDate),
	repayments_(repayments){
	QL_REQUIRE(maturity_ > 0.0, "maturity must be positive");
	QL_REQUIRE(strike_ > 0.0, "strike must be positive");
}

Real AutocallablePathPricer::operator()(const MultiPath& paths) const {

	const MultiPath& path= paths;
	//const Path& path = paths[0];

	Calendar calendar = TARGET();
	DayCounter dayCount = ActualActual();

	Real startinglevel = strike_;
	Real excerciselevel = 15.08;
	Real barrierlevel = 9.0504;
	Date barrierDate(01, March, 2021);
	Real plus = 58;
	Date plusDate = repayments_.front().paymentDate;

	//initialization of the price
	Real price = plus * OISTermStructure_->discount(plusDate);

	auto repayment = occurredRepayment(repayments_, path[0], dayCount, settlementDate_);
	price += repayment.value;
	if (repayment.paymentDate == repayments_.back().paymentDate) {
		auto stock = stockValue(path[0], repayment.evaluationDates.back(), dayCount, settlementDate_);
		if (stock < barrierlevel) {
			price -= repayment.coupon * OISTermStructure_->discount(repayment.paymentDate);
			auto faceNPV = repayment.value - repayment.coupon * OISTermStructure_->discount(repayment.paymentDate);
			auto stock_performance = computeAverage(repayments_.back(), path[0], dayCount, settlementDate_);
			price -= faceNPV * (1 - stock_performance / startinglevel);
		}
	}
	return price;
}

Repayment occurredRepayment(const std::vector<Repayment>& repayments,
	const Path& stockPath,
	const DayCounter& dayCount,
	const Date& settlementDate) {
	for (auto const& r : repayments) {
		Real average = computeAverage(r, stockPath, dayCount, settlementDate);
		if (average >= r.exerciseLevel){
			return r;
		}
	}
	return repayments.back();
}
	
Real stockValue(const Path& path, const Date& date,
				const DayCounter& dayCount, const Date& settlementDate) {
	auto dateTime = dayCount.yearFraction(settlementDate, date);
	return path.at(path.timeGrid().closestIndex(dateTime));
}

Real computeAverage(const Repayment& r, const Path& stockPath, 
	const DayCounter& dayCount,	const Date& settlementDate) {
	Real average = 0;
	for (auto const& obserDate : r.evaluationDates) {
		average += stockValue(stockPath, obserDate, dayCount, settlementDate);
	}
	average = average / r.evaluationDates.size();
	return average;
}
