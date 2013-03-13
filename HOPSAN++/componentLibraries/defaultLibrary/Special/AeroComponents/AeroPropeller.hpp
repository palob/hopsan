#ifndef AEROPROPELLER_HPP_INCLUDED
#define AEROPROPELLER_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file AeroPropeller.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 6 Nov 2012 20:53:56
//! @brief Model of a propeller
//! @ingroup AeroComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Users, petkr14.IEI, Documents, CompgenNG}/AeroComponentsNG.nb*/

using namespace hopsan;

class AeroPropeller : public ComponentC
{
private:
     double mdp;
     double mb1;
     double mb2;
     double mg1;
     double mg2;
     double mct0;
     double mcp0;
     double mk;
     double mUp;
     double mrho;
     Port *mpPmr1;
     Port *mpPUp;
     Port *mpPrho;
     Port *mpPthrust;
     Port *mpPtorque;
     Port *mpPPin;
     Port *mpPPout;
     Port *mpPJp;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[2];
     int mNstep;
     //Port Pmr1 variable
     double tormr1;
     double thetamr1;
     double wmr1;
     double cmr1;
     double Zcmr1;
     double eqInertiamr1;
     //inputVariables
     double Up;
     double rho;
     //outputVariables
     double thrust;
     double torque;
     double Pin;
     double Pout;
     double Jp;

     //Expressions variables
     //Port Pmr1 pointer
     double *mpND_tormr1;
     double *mpND_thetamr1;
     double *mpND_wmr1;
     double *mpND_cmr1;
     double *mpND_Zcmr1;
     double *mpND_eqInertiamr1;
     //Delay declarations
     //inputVariables pointers
     double *mpND_Up;
     double *mpND_rho;
     //outputVariables pointers
     double *mpND_thrust;
     double *mpND_torque;
     double *mpND_Pin;
     double *mpND_Pout;
     double *mpND_Jp;
     Delay mDelayedPart10;
     Delay mDelayedPart20;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new AeroPropeller();
     }

     void configure()
     {
        const double dp = 1.;
        const double b1 = 0.2;
        const double b2 = 0.2;
        const double g1 = 0.205;
        const double g2 = 0.2;
        const double ct0 = 0.12;
        const double cp0 = 0.08;
        const double k = 4;
        const double Up = 1.25;
        const double rho = 1.25;

        mNstep=9;
        jacobianMatrix.create(2,2);
        systemEquations.create(2);
        delayedPart.create(3,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;

        mdp = dp;
        mb1 = b1;
        mb2 = b2;
        mg1 = g1;
        mg2 = g2;
        mct0 = ct0;
        mcp0 = cp0;
        mk = k;
        mUp = Up;
        mrho = rho;

        //Add ports to the component
        mpPmr1=addPowerPort("Pmr1","NodeMechanicRotational");

        //Add inputVariables ports to the component
        mpPUp=addReadPort("PUp","NodeSignal", Port::NotRequired);
        mpPrho=addReadPort("Prho","NodeSignal", Port::NotRequired);

        //Add outputVariables ports to the component
        mpPthrust=addWritePort("Pthrust","NodeSignal", Port::NotRequired);
        mpPtorque=addWritePort("Ptorque","NodeSignal", Port::NotRequired);
        mpPPin=addWritePort("PPin","NodeSignal", Port::NotRequired);
        mpPPout=addWritePort("PPout","NodeSignal", Port::NotRequired);
        mpPJp=addWritePort("PJp","NodeSignal", Port::NotRequired);

        //Register changable parameters to the HOPSAN++ core
        registerParameter("dp", "Propeller diameter", "m", mdp);
        registerParameter("b1", "Propeller thrust coefficient", "m", mb1);
        registerParameter("b2", "Propeller thrust coefficient", "m", mb2);
        registerParameter("g1", "Propeller torque coefficient", "m", mg1);
        registerParameter("g2", "Propeller torque coefficient", "m", mg2);
        registerParameter("ct0", "Propeller torque coefficient", "m", mct0);
        registerParameter("cp0", "Propeller torque coefficient", "m", mcp0);
        registerParameter("k", "exponent for transition", "", mk);
        registerParameter("Up", "Air speed", "m/s", mUp);
        registerParameter("rho", "Air density", "kg/m3", mrho);
        mpSolver = new EquationSystemSolver(this,2);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pmr1
        mpND_tormr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::TORQUE);
        mpND_thetamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::ANGLE);
        mpND_wmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::ANGULARVELOCITY);
        mpND_cmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::WAVEVARIABLE);
        mpND_Zcmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::CHARIMP);
        mpND_eqInertiamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::EQINERTIA);
        //Read inputVariables pointers from nodes
        mpND_Up=getSafeNodeDataPtr(mpPUp, NodeSignal::VALUE,mUp);
        mpND_rho=getSafeNodeDataPtr(mpPrho, NodeSignal::VALUE,mrho);
        //Read outputVariable pointers from nodes
        mpND_thrust=getSafeNodeDataPtr(mpPthrust, NodeSignal::VALUE);
        mpND_torque=getSafeNodeDataPtr(mpPtorque, NodeSignal::VALUE);
        mpND_Pin=getSafeNodeDataPtr(mpPPin, NodeSignal::VALUE);
        mpND_Pout=getSafeNodeDataPtr(mpPPout, NodeSignal::VALUE);
        mpND_Jp=getSafeNodeDataPtr(mpPJp, NodeSignal::VALUE);

        //Read variables from nodes
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);

        //Read inputVariables from nodes
        Up = (*mpND_Up);
        rho = (*mpND_rho);

        //Read outputVariables from nodes
        thrust = mpPthrust->getStartValue(NodeSignal::VALUE);
        torque = mpPtorque->getStartValue(NodeSignal::VALUE);
        Pin = mpPPin->getStartValue(NodeSignal::VALUE);
        Pout = mpPPout->getStartValue(NodeSignal::VALUE);
        Jp = mpPJp->getStartValue(NodeSignal::VALUE);



        //Initialize delays

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(2);
        Vec stateVark(2);
        Vec deltaStateVar(2);

        //Read variables from nodes
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);

        //Read inputVariables from nodes
        Up = (*mpND_Up);
        rho = (*mpND_rho);

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = thrust;
        stateVark[1] = cmr1;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //Propeller
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =thrust + \
(Power(mdp,3)*rho*(0.025330295910584444*mb2*Up + \
mb1*mdp*(-2.5330295910584445e-7 - \
0.004031441804149936*wmr1))*Power(wmr1,2)*Power(1/(Power(1/mct0,mk) + \
Power(Abs(mb1 - (1.*mb2*Up)/(0.00001*mdp + \
0.15915494309189535*mdp*wmr1)),-mk)),1/mk))/((0.00001 + \
0.15915494309189535*wmr1)*Abs(mb1 - (1.*mb2*Up)/(0.00001*mdp + \
0.15915494309189535*mdp*wmr1)));
          systemEquations[1] =cmr1 + \
(Power(mdp,4)*rho*(0.004031441804149936*mg2*Up + \
mdp*mg1*(-4.031441804149937e-8 - \
0.000641623890917771*wmr1))*Power(wmr1,2)*Power(1/(Power(1/mcp0,mk) + \
Power(Abs(mg1 - (1.*mg2*Up)/(0.00001*mdp + \
0.15915494309189535*mdp*wmr1)),-mk)),1/mk))/((0.00001 + \
0.15915494309189535*wmr1)*Abs(mg1 - (1.*mg2*Up)/(0.00001*mdp + \
0.15915494309189535*mdp*wmr1)));

          //Jacobian matrix
          jacobianMatrix[0][0] = 1 + 0.*Power(1/(Power(1/mct0,mk) + \
Power(Abs(mb1 - (1.*mb2*Up)/(0.00001*mdp + \
0.15915494309189535*mdp*wmr1)),-mk)),1/mk);
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[1][0] = 0.*Power(1/(Power(1/mcp0,mk) + Power(Abs(mg1 \
- (1.*mg2*Up)/(0.00001*mdp + 0.15915494309189535*mdp*wmr1)),-mk)),1/mk);
          jacobianMatrix[1][1] = 1;

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          thrust=stateVark[0];
          cmr1=stateVark[1];
          //Expressions
          torque = cmr1;
          Zcmr1 = 0.;
          Pin = cmr1*wmr1;
          Pout = thrust*Up;
          Jp = Up/(mdp*(0.00001 + 0.159155*wmr1));
        }

        //Calculate the delayed parts

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];

        //Write new values to nodes
        //Port Pmr1
        (*mpND_cmr1)=cmr1;
        (*mpND_Zcmr1)=Zcmr1;
        //outputVariables
        (*mpND_thrust)=thrust;
        (*mpND_torque)=torque;
        (*mpND_Pin)=Pin;
        (*mpND_Pout)=Pout;
        (*mpND_Jp)=Jp;

        //Update the delayed variabels

     }
};
#endif // AEROPROPELLER_HPP_INCLUDED
