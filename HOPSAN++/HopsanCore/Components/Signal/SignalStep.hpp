//!
//! @file   SignalStep.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-01-08
//!
//! @brief Contains a step signal generator
//!
//$Id$

///////////////////////////////////////////
//                    XXXXXX  ↑          //
//                    X       | StepSize //
//                    X       |          //
// StartValue →  XXXXXX       ↓          //
//                                       //
//                    ↑                  //
//                 StepTime              //
///////////////////////////////////////////

#ifndef SIGNALSTEP_HPP_INCLUDED
#define SIGNALSTEP_HPP_INCLUDED

#include "../../ComponentEssentials.h"

//!
//! @brief
//! @ingroup SignalComponents
//!
class SignalStep : public ComponentSignal
{

private:
    double mBaseValue;
    double mAmplitude;
    double mStepTime;
    Port *mpOut;

public:
    static Component *Creator()
    {
        std::cout << "running Step creator" << std::endl;
        return new SignalStep("Step");
    }


    SignalStep(const string name,
                          const double basevalue = 0.0,
                          const double amplitude = 1.0,
                          const double steptime = 1.0,
                          const double timestep = 0.001)
	: ComponentSignal(name, timestep)
    {
        mTypeName = "SignalStep";
        mBaseValue = basevalue;
        mAmplitude = amplitude;
        mStepTime = steptime;

        mpOut = addWritePort("out", "NodeSignal");

        registerParameter("BaseValue", "Base Value", "-", mBaseValue);
        registerParameter("Amplitude", "Amplitude", "-", mAmplitude);
        registerParameter("StepTime", "Step Time", "-", mStepTime);
    }


    void initialize()
    {
        mpOut->writeNode(NodeSignal::VALUE, mBaseValue);
    }


    void simulateOneTimestep()
    {
        //Step Equations
        double output;
        if (mTime <= mStepTime)
        {
            output = mBaseValue;     //Before step
        }
        else
        {
            output = mBaseValue + mAmplitude;     //After step
        }

        //Write new values to nodes
        mpOut->writeNode(NodeSignal::VALUE, output);
    }
};

#endif // SIGNALSTEP_HPP_INCLUDED
