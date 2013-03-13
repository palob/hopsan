#ifndef ELECTRICUSOURCE_HPP_INCLUDED
#define ELECTRICUSOURCE_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file ElectricUsource.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 31 Jan 2012 15:03:07
//! @brief An inertia load with spring and damper
//! @ingroup ElectricComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/ElectricNG.nb*/

using namespace hopsan;

class ElectricUsource : public ComponentC
{
private:
     double muin;
     Port *mpPel1;
     Port *mpPuin;
     int mNstep;
     //Port Pel1 variable
     double uel1;
     double iel1;
     double cel1;
     double Zcel1;
     //inputVariables
     double uin;
     //outputVariables

     //Expressions variables

     //Port Pel1 pointer
     double *mpND_uel1;
     double *mpND_iel1;
     double *mpND_cel1;
     double *mpND_Zcel1;
     //Delay declarations
     //inputVariables pointers
     double *mpND_uin;
     //outputVariables pointers

public:
     static Component *Creator()
     {
        return new ElectricUsource();
     }

     void configure()
     {
        const double uin = 12.;

        mNstep=9;
        muin = uin;

        //Add ports to the component
        mpPel1=addPowerPort("Pel1","NodeElectric");

        //Add inputVariables ports to the component
        mpPuin=addReadPort("Puin","NodeSignal", Port::NotRequired);

        //Add outputVariables ports to the component

        //Register changable parameters to the HOPSAN++ core
        registerParameter("uin", "Voltage", "V", muin);
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
        mpND_uin=getSafeNodeDataPtr(mpPuin, NodeSignal::VALUE,muin);
        //Read outputVariable pointers from nodes

        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);

        //Read inputVariables from nodes
        uin = (*mpND_uin);

        //Read outputVariables from nodes



        //Initialize delays

     }
    void simulateOneTimestep()
     {
        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);

        //Read inputVariables from nodes
        uin = (*mpND_uin);

        //LocalExpressions

          //Expressions
          cel1 = uin;
          Zcel1 = 0.;

        //Calculate the delayed parts


        //Write new values to nodes
        //Port Pel1
        (*mpND_cel1)=cel1;
        (*mpND_Zcel1)=Zcel1;
        //outputVariables

        //Update the delayed variabels

     }
};
#endif // ELECTRICUSOURCE_HPP_INCLUDED
