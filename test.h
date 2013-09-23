
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <ql/quantlib.hpp>
#include "CIR2p.h"
#include <boost/random.hpp>
//#include <boost/filesystem.hpp>
	

//namespace fs=boost::filesystem;
using namespace QuantLib;
/*
void simulateYieldCIR2p(CIR2p* myCIR2p, std::vector<Date>& simulationDates, std::vector<Real>& tenor, Integer nbSims, std::string fileName){
	std::ofstream myfile;
		myfile.open (fileName);

		//We define the generator of normal variable
		Real seed=12345;
		boost::mt19937 rng(seed);
		boost::normal_distribution<> normal;
		boost::variate_generator<boost::mt19937&,boost::normal_distribution<>> simuNormal(rng,normal);
		for (int i=0;i<nbSims;i++){
			//The first date is assume to be t0

			for(int j=0;j<simulationDates.size;j++){			
				if(j=0){
					dt=0;
				} else{
					dt=simulationDates[j]-simulationDates[j-1];
				}
				ti=simulationDates[j];
				x=myP.evolve(ti,x,dt,dw);
				std::cout << "simu " << i << " - dw[0]=" << dw[0] << " - dw[1]=" << dw[1] << std::endl;
				std::cout << "simu " << i << " - x=" << x[0] << " - y=" << x[1] << std::endl;
				ti+=dt;
				dw[0]=simuNormal();
				dw[1]=simuNormal();
			}

		}
	
		myfile << "Writing ths to a file.\n";
		myfile.close();
}

int test1()
{
	
	Real x0=0;
	Real y0=0;
	Real a1=0.1;
	Real a2=0.2;
	Real b1=0.02;
	Real b2=0.04;
	Array startingPoints(2);
	startingPoints[0]=x0;
	startingPoints[1]=y0;
	Volatility sigma1=0.15;
	Volatility sigma2=0.2;


	Date settlementDate(Settings::instance().evaluationDate());
        
    DayCounter dayCounter = Actual365Fixed();
    Calendar calendar = TARGET();
    
	int nbRates = 8;
    Integer t[] = { 13, 41, 75, 165, 256, 345, 524, 703 };
    Rate r[] = { 0.0357,0.0349,0.0341,0.0355,0.0359,0.0368,0.0386,0.0401 };
	DiscountFactor df[] = {0.998729301173014,0.996087400269755,0.993017641419248,0.984080136999817,0.975135173523875,0.965814432925361,0.946092632512588,0.925673499697841}; // df corresponding to the rates r

    std::vector<Date> dates;
    std::vector<Rate> rates;
    std::vector<DiscountFactor> dfs;
    dates.push_back(settlementDate);
    rates.push_back(0.0357);
	dfs.push_back(1.0);

    Size i;
    for (i = 0; i < nbRates ; i++) {
        dates.push_back(settlementDate + t[i]);
        rates.push_back(r[i]);
		dfs.push_back(df[i]);
    }


	InterpolatedDiscountCurve <LogLinear> * curve=new DiscountCurve(dates, dfs ,dayCounter, calendar );
	boost::shared_ptr<YieldTermStructure> myCurve(curve);

	Handle<YieldTermStructure> h(myCurve);

	CIR2p myCIR2P( x0,  y0,  a1,  a2,  b1,  b2,  sigma1,  sigma2, h , CIR::FullTruncation);
	CIR myP( x0, a1, b1, sigma1, CIR::FullTruncation );


	Real dt=0.0192;
	Real t0=0;
	Real ti=t0;

	/*
		//FOR CIR2p

	Array x(2,(x0,y0));
	Array dw(2,0);

	//Random generator
	Real seed=12345;
	boost::mt19937 rng(seed);
	boost::normal_distribution<> normal;
	boost::variate_generator<boost::mt19937&,boost::normal_distribution<>> simuNormal(rng,normal);
	for(int i=0;i<10;i++)
	{
		x=myP.evolve(ti,x,dt,dw);
		std::cout << "simu " << i << " - dw[0]=" << dw[0] << " - dw[1]=" << dw[1] << std::endl;
		std::cout << "simu " << i << " - x=" << x[0] << " - y=" << x[1] << std::endl;
		ti+=dt;
		dw[0]=simuNormal();
		dw[1]=simuNormal();
	}
	


	for(int i=0;i<nbRates;i++){
		std::cout << "simu " << myCIR2P.yield((double)t[i]/365.0, startingPoints) << ", expected: " << h->zeroRate(t[i]/365.0,Continuous) << std::endl;	
		
	}
	return 0;
}

int testtest(){
	Real a= 1;
	std::cout << a / 2 << std::endl;
	std::cout << a / 2.0 << std::endl;
	std::cout << 1 / 2.0 << std::endl;
	std::cout << 2.0 * 1 / 4 << std::endl;
	std::cout << 1 / 4 * 2.0 << std::endl;
	return 0;
	
}



class myClass{
public:
	int i_;
	myClass(int i){
		i_=i;
		std::cout << "constructeur called for object " << i_ << std::endl ;}
	~myClass(){std::cout << "destructeur called for object " << i_ << std::endl ;}
};

int ptr_test(){
	boost::shared_ptr<myClass> A(new myClass(1));
	myClass* obj = new myClass(2);
	boost::shared_ptr<myClass> B(obj);
	myClass obj2(3);
	boost::shared_ptr<myClass> C((myClass*)&obj2);
	return 0;
}*/
