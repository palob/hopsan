/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   MechanicAngularVelocityTransformer.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-08-05
//!
//! @brief Contains an angular velocity transformer component
//!
//$Id$

#ifndef MECHANICANGULARVELOCITYTRANSFORMER_HPP_INCLUDED
#define MECHANICANGULARVELOCITYTRANSFORMER_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup MechanicalComponents
    //!
    class MechanicAngularVelocityTransformer : public ComponentQ
    {

    private:
        double *mpW;
        double *mpND_t, *mpND_a, *mpND_w, *mpND_c, *mpND_Zx;
        Integrator mInt;
        Port *mpIn, *mpOut;

    public:
        static Component *Creator()
        {
            return new MechanicAngularVelocityTransformer();
        }

        void configure()
        {
            mpIn = addReadPort("in", "NodeSignal", Port::NotRequired);
            mpOut = addPowerPort("out", "NodeMechanicRotational");
            addInputVariable("omega", "Generated angular velocity", "[rad/s]", 0.0, &mpW);
        }


        void initialize()
        {
            mpND_t = getSafeNodeDataPtr(mpOut, NodeMechanicRotational::Torque);
            mpND_a = getSafeNodeDataPtr(mpOut, NodeMechanicRotational::Angle);
            mpND_w = getSafeNodeDataPtr(mpOut, NodeMechanicRotational::AngularVelocity);
            mpND_c = getSafeNodeDataPtr(mpOut, NodeMechanicRotational::WaveVariable);
            mpND_Zx = getSafeNodeDataPtr(mpOut, NodeMechanicRotational::CharImpedance);

            mInt.initialize(mTimestep, (*mpW), 0.0);
        }


        void simulateOneTimestep()
        {
            //Get variable values from nodes
            double w, c, Zx, a, t;
            w = (*mpW);
            c = (*mpND_c);
            Zx = (*mpND_Zx);

            //Spring equations
            a = mInt.update(w);
            t = c + Zx*w;

            //Write values to nodes
            (*mpND_t) = t;
            (*mpND_a) = a;
            (*mpND_w) = w;
        }
    };
}

#endif // MECHANICANGULARVELOCITYTRANSFORMER_HPP_INCLUDED




