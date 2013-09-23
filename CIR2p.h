#pragma once

#include <boost/shared_ptr.hpp>
#include <ql/quantlib.hpp>
#include <ql/stochasticprocess.hpp>

namespace QuantLib{

	/*
		First we define a CIR class heriting from SquareRootProcess but in which we will define a much broader 'evolve' method (with different truncation method) 
		and create other method related to the valuation of a bond if a rate is simulated as CIR
		A CIR process is define by the following formula
			dx(t)    = a[b-x(t)]dt + sigma dWt
	*/
	class CIR: public StochasticProcess1D{
	public:
		//'Discretization' is totally different from StochasticProcess1D::discretization, it is used to indicates what type of discretization we will use for the CIR process (in 'SquareRootProcess' the only possible ones are 
		//'EulerDiscretization' or 'EndEulerDiscretization')
		enum Discretization { PartialTruncation,
							  FullTruncation,
							  Reflection};
		
		CIR(Real x0, Real a, Real b, Volatility sigma, Discretization d=FullTruncation); 
        //! \name CIR interface
        //@{
        Real x0() const;
        Real drift(Time t, Real x) const;
        Real diffusion(Time t, Real x) const;
		Real evolve(Time t0, Real x0, Time dt, Real dw);  
		Real bondPrice(Time t, Time T, Real x);
		Real yield(Real tenor, Real x);
		Real yield(Real tenor){return yield(tenor,x0_);};
        //@}
	private:
		Real A(Time t, Time T);
		Real B(Time t, Time T);
	protected:
        Real x0_, mean_, speed_;
        Volatility volatility_;
		Discretization d_;
	};

	/*
		CIR 2 Factors model with NO correl between the BM. We adopt the same notations than for the QL implementqtion of CIR contrary to the ones used in Brigo and Mercurio book
		The equation followed by r is:
			   r(t)=x(t)+y(t)+theta(t)
		with   dx(t)    = a1[b1-x(t)]dt + sigma1 dWt
			   dy(t)    = a2[b2-y(t)]dt + sigma2 dZt
			   d<Wt,Zt> = 0

		Given a1,a2,sigma1,sigma2 and the initial term structure 'h', the component exp(-int of theta(s) between t and T) is calculated on the fly
	*/

	class CIR2p{
	protected:
			boost::shared_ptr<CIR> xProcess_;
			boost::shared_ptr<CIR> yProcess_;
			Handle<YieldTermStructure> h_;
	public:
		CIR2p(Real x0, Real y0, Real a1, Real a2, Real b1, Real b2, Volatility sigma1, Volatility sigma2, Handle<YieldTermStructure>& h, CIR::Discretization d=CIR::Discretization::FullTruncation); 

		Disposable<Array> evolve(Time t0, Array& x0, Time dt, Array& dw); //It evolves only xProcess and yProcess. Theta is disregarded. 
		//Disposable<Array> apply(Array& x0, Array& dx);
		Real bondPrice(Time t, Time T, Array& x);
		Real yield(Real tenor, Array& x);
		Real yield(Time t, Time T, Array& x);
		Real yield(Real tenor);
	private:
		Real sumTheta(Time t, Time T, Array& x); //return the component exp(-int of theta(s) between t and mat), assuming that the forward rates are constant, as described p98 in Brigo Mercurio Book
	};
}

