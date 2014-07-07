//!
//! @file   MechanicRotationalInertiaWithGearRatio.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2011-03-15
//!
//! @brief Contains a mechanic rotational gear ratio with inertia component
//!
//$Id$

#ifndef MECHANICROTATIONALINERTIAWITHGEARRATIO_HPP_INCLUDED
#define MECHANICROTATIONALINERTIAWITHGEARRATIO_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup MechanicalComponents
    //! @details
    //! Implements the following motion equations
    //! \f{eqnarray*}{
    //! J\dot{\omega}_{2} + B\omega_{2} &=& UT_{1}-T_{2} \\
    //! \omega_{1} &=& -U\omega_{2} \\
    //! \theta_{1} &=& -U\theta_{2}
    //! \f}
    //!
    class MechanicRotationalInertiaWithGearRatio : public ComponentQ
    {

    private:
        double *mpU, *mpB;
        double J, k;
        double mNumTheta[3];
        double mDenTheta[3];
        double mNumOmega[2];
        double mDenOmega[2];
        SecondOrderTransferFunction mThetaFilter;
        FirstOrderTransferFunction mOmegaFilter;

        double *mpP1_t, *mpP1_a, *mpP1_w, *mpP1_c, *mpP1_Zx,
               *mpP2_t, *mpP2_a, *mpP2_w, *mpP2_c, *mpP2_Zx;
        Port *mpP1, *mpP2;

    public:
        static Component *Creator()
        {
            return new MechanicRotationalInertiaWithGearRatio();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeMechanicRotational");
            mpP2 = addPowerPort("P2", "NodeMechanicRotational");
            addInputVariable("omega", "Gear ratio", "-", 1.0, &mpU);
            addInputVariable("B", "Viscous Friction", "Nms/rad", 10.0, &mpB);
            addConstant("J", "Moment of Inertia", "kgm^2", 1.0, J);
        }

        void initialize()
        {
            mpP1_t = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::Torque);
            mpP1_a = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::Angle);
            mpP1_w = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::AngularVelocity);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::WaveVariable);
            mpP1_Zx = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::CharImpedance);

            mpP2_t = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::Torque);
            mpP2_a = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::Angle);
            mpP2_w = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::AngularVelocity);
            mpP2_c = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::WaveVariable);
            mpP2_Zx = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::CharImpedance);

            double t1, t2, a2, w2, U, B;
            B = (*mpB);
            U = (*mpU);
            t1 = (*mpP1_t);
            t2 = (*mpP2_t);
            a2 = (*mpP2_a);
            w2 = (*mpP2_w);

            mNumTheta[0] = 1.0;
            mNumTheta[1] = 0.0;
            mNumTheta[2] = 0.0;
            mDenTheta[0] = 0;
            mDenTheta[1] = B;
            mDenTheta[2] = J;
            mNumOmega[0] = 1.0;
            mNumOmega[1] = 0.0;
            mDenOmega[0] = B;
            mDenOmega[1] = J;

            mThetaFilter.initialize(mTimestep, mNumTheta, mDenTheta, U*t1-t2, a2, -1.5e300, 1.5e300, w2);
            mOmegaFilter.initialize(mTimestep, mNumOmega, mDenOmega, U*t1-t2, w2);
        }


        void simulateOneTimestep()
        {
            double t1, a1, w1, c1, Zx1, t2, a2, w2, c2, Zx2, U, B;

            //Get variable values from nodes
            U = (*mpU);
            B = (*mpB);
            c1  = (*mpP1_c);
            Zx1 = (*mpP1_Zx);
            c2  = (*mpP2_c);
            Zx2 = (*mpP2_Zx);

            //Mass equations
            const double BB = B+U*U*Zx1+Zx2;
            mDenTheta[1] = BB;
            mDenOmega[0] = BB;
            mThetaFilter.setDen(mDenTheta);
            mOmegaFilter.setDen(mDenOmega);

            const double TT = U*c1-c2;
            a2 = mThetaFilter.update(TT);
            w2 = mOmegaFilter.update(TT);
            t2 = c2 + Zx2*w2;

            w1 = -w2*U;
            a1 = -a2*U;
            t1 = c1 + Zx1*w1;

            //Write new values to nodes
            (*mpP1_t) = t1;
            (*mpP1_a) = a1;
            (*mpP1_w) = w1;
            (*mpP2_t) = t2;
            (*mpP2_a) = a2;
            (*mpP2_w) = w2;
        }
    };
}

#endif // MECHANICROTATIONALINERTIAWITHGEARRATIO_HPP_INCLUDED

