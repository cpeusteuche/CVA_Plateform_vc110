#include "stdafx.h"
#include "CIR2p.h"



namespace QuantLib {
	/******************************************************************
		CIR
	 ******************************************************************/
	CIR::CIR(Real x0, Real a, Real b, Volatility sigma, Discretization d):
		StochasticProcess1D(boost::shared_ptr<discretization>(new EulerDiscretization)),
		x0_(x0),
		speed_(a),
		mean_(b),
		volatility_(sigma),
		d_(d){}	

    Real CIR::x0() const {
		switch (d_) {
          case PartialTruncation:
			  return x0_;
          case FullTruncation:
			  return x0_;
          case Reflection:
			  return std::fabs(x0_);
          default:
            QL_FAIL("unknown discretization schema");
        }

      }

    Real CIR::drift(Time, Real x) const {
		switch (d_) {
          case PartialTruncation:
			  return speed_ * ( mean_ - x );
          case FullTruncation:
			  return speed_ * ( mean_ - std::max(x,0.0) );
          case Reflection:
			  return speed_ * ( mean_ - std::fabs(x) );
          default:
            QL_FAIL("unknown discretization schema");
		}
    }

    Real CIR::diffusion(Time, Real x) const {
		switch (d_) {
          case PartialTruncation:
			  return volatility_*std::sqrt( std::max(x,0.0) );
          case FullTruncation:
			  return volatility_*std::sqrt( std::max(x,0.0) );
          case Reflection:
			  return volatility_*std::sqrt( std::fabs(x) );
          default:
            QL_FAIL("unknown discretization schema");
		}
    }


	Real CIR::evolve(Time t0, Real x0, Time dt, Real dw) {
         return this->x0() + drift(t0,x0) * dt + diffusion(t0,x0) * std::sqrt(dt) * dw;
    }

	Real CIR::A(Time t, Time T){
		Real h = std::sqrt( speed_*speed_ + 2 * volatility_*volatility_ );
		return std::pow( 
						 (2 * h * std::exp( (h + speed_) * (T - t) * 0.5 ) )/
						 (2 * h + (speed_ + h) * (std::exp( (T - t) * h) - 1))
						 ,
					    ( (2 * speed_ * mean_)/(volatility_*volatility_) )
					   );
	}
	Real CIR::B(Time t, Time T){
		Real h = std::sqrt( speed_*speed_ + 2 * volatility_*volatility_ );
		return (2.0 * (std::exp(h * (T - t)) - 1) )/
			   (2.0 * h + (speed_ + h) * (std::exp( (T - t) * h) - 1));
	}

	Real CIR::bondPrice(Time t, Time T, Real x){
		return A(t,T) * std::exp(-B(t,T) * x);
	}

	Real CIR::yield(Real tenor, Real x){
		return -1.0/tenor * std::log(bondPrice(0,tenor,x));
	}


	/******************************************************************
		CIR2p
	 ******************************************************************/
	CIR2p::CIR2p(Real x0, Real y0, Real a1, Real a2, Real b1, Real b2, Volatility sigma1, Volatility sigma2, Handle<YieldTermStructure>& h, CIR::Discretization d):
		xProcess_( new CIR(x0, a1, b1, sigma1, d) ),
		yProcess_( new CIR(y0, a2, b2, sigma2, d) ),
		h_(h){}	

	Real CIR2p::bondPrice(Time t, Time T, Array& x){
		std::cout << "x=" << xProcess_->bondPrice(t,T,x[0]) << std::endl;
		std::cout << "y=" << yProcess_->bondPrice(t,T,x[1]) << std::endl;
		std::cout << "sumTheta=" <<  sumTheta(t,T,x) << std::endl;
		
		return xProcess_->bondPrice(t,T,x[0]) *
			   yProcess_->bondPrice(t,T,x[1]) *
			   sumTheta(t,T,x); 
	}

	//return the component exp(-int of theta(s) between t and mat), assuming that the forward rates are constant, as described p98 in Brigo Mercurio Book
	Real CIR2p::sumTheta(Time t, Time T, Array& x){
		return xProcess_->bondPrice(0,t,x[0]) * yProcess_->bondPrice(0,t,x[1]) / //P(0,t)
			   (xProcess_->bondPrice(0,T,x[0]) * yProcess_->bondPrice(0,T,x[1])) * //P(0,T)
			   std::exp( - h_->zeroRate(T,Continuous) * T ) / //P_Market(0,T)
			   std::exp( - h_->zeroRate(t,Continuous) * t ) ; //P_Market(0,T)
	}

	Real CIR2p::yield(Real tenor, Array& x){
		return -1.0/tenor * std::log(bondPrice(0,tenor,x));
	}
	
	Real CIR2p::yield(Time t, Time T, Array& x){
		return -1.0/(T-t) * std::log(bondPrice(t,T,x));
	}

	Real CIR2p::yield(Real tenor){
		Array x(2);
		x[0]=xProcess_->x0();
		x[1]=yProcess_->x0();
		return this->yield(tenor,x);
	}

	Disposable<Array> CIR2p::evolve(Time t0, Array& x0, Time dt, Array& dw){
		Array xEvolve(2);
		xEvolve[0]=this->xProcess_->evolve(t0,x0[0],dt,dw[0]);
		xEvolve[1]=this->yProcess_->evolve(t0,x0[1],dt,dw[1]);
		return xEvolve;
	}

	/*
	Disposable<Array> CIR2p::apply(Array& x0, Array& dx){
		h_->
		return x0+dx;
	}
	*/
}