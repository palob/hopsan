#ifndef ELECTRICISENSOR_HPP_INCLUDED
#define ELECTRICISENSOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file ElectricIsensor.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 31 Jan 2012 15:03:09
//! @brief Sensor of electric current
//! @ingroup ElectricComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/ElectricNG.nb*/

using namespace hopsan;

class ElectricIsensor : public ComponentSignal
{
private:
     Port *mpPel1;
     Port *mpPiout;
     int mNstep;
     //Port Pel1 variable
     double uel1;
     double iel1;
     double cel1;
     double Zcel1;
     //inputVariables
     //outputVariables
     double iout;

     //Expressions variables

     //Port Pel1 pointer
     double *mpND_uel1;
     double *mpND_iel1;
     double *mpND_cel1;
     double *mpND_Zcel1;
     //Delay declarations
     //inputVariables pointers
     //outputVariables pointers
     double *mpND_iout;

public:
     static Component *Creator()
     {
        return new ElectricIsensor();
     }

     void configure()
     {

        mNstep=9;

        //Add ports to the component
        mpPel1=addReadPort("Pel1","NodeElectric");

        //Add inputVariables ports to the component

        //Add outputVariables ports to the component
        mpPiout=addWritePort("Piout","NodeSignal", Port::NotRequired);

        //Register changable parameters to the HOPSAN++ core
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pel1
        mpND_uel1=getSafeNodeDataPtr(mpPel1, NodeElectric::VOLTAGE);
        mpND_iel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CURRENT);
        mpND_cel1=getSafeNodeDataPtr(mpPel1, NodeElectric::WAVEVARIABLE);
        mpND_Zcel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CHARIMP);
        //Read inputVariables pointers from nodes
        //Read outputVariable pointers from nodes
        mpND_iout=getSafeNodeDataPtr(mpPiout, NodeSignal::VALUE);

        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);

        //Read inputVariables from nodes

        //Read outputVariables from nodes
        iout = mpPiout->getStartValue(NodeSignal::VALUE);



        //Initialize delays

     }
    void simulateOneTimestep()
     {
        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);

        //Read inputVariables from nodes

        //LocalExpressions

          //Expressions
          iout = iel1;

        //Calculate the delayed parts

        //Write new values to nodes

        //outputVariables
        (*mpND_iout)=iout;

        //Update the delayed variabels

     }
};
#endif // ELECTRICISENSOR_HPP_INCLUDED
