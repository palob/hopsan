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
//! @file   Hydraulic43ValveNeutralToTank.hpp
//! @author Karl Pettersson <karl.pettersson@liu.se>
//! @date   2010-01-12
//!
//! @brief Contains a hydraulic 4/3-valve of Q-type
#ifndef HYDRAULIC43ValveNeutralToTankNEUTRALTOTANK_HPP_INCLUDED
#define HYDRAULIC43ValveNeutralToTankNEUTRALTOTANK_HPP_INCLUDED



#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief Hydraulic 4/3-valve (closed centre) of Q-type.
    //! @ingroup HydraulicComponents
    //!
    class Hydraulic43ValveNeutralToTank : public ComponentQ
    {
    private:
        double Cq;
        double d;
        double f_pa, f_pb, f_at, f_bt, f_c;
        double p_c;
        double xvmax;
        double rho;
        double overlap_pa;
        double overlap_pb;
        double overlap_at;
        double overlap_bt;
        double omegah;
        double deltah;

        double *mpND_pp, *mpND_qp, *mpND_pt, *mpND_qt, *mpND_pa, *mpND_qa, *mpND_pb, *mpND_qb;
        double *mpND_cp, *mpND_Zcp, *mpND_ct, *ZmpND_ct, *mpND_ca, *mpND_Zca, *mpND_cb, *mpND_Zcb;
        double *mpND_xvin, *mpND_xvout;

        SecondOrderTransferFunction filter;
        TurbulentFlowFunction qTurb_pa;
        TurbulentFlowFunction qTurb_pb;
        TurbulentFlowFunction qTurb_at;
        TurbulentFlowFunction qTurb_bt;
        Port *mpPP, *mpPT, *mpPA, *mpPB, *mpIn, *mpOut;

    public:
        static Component *Creator()
        {
            return new Hydraulic43ValveNeutralToTank();
        }

        void configure()
        {
            Cq = 0.67;
            d = 0.01;
            p_c = 0.02;
            f_pa = 1.0;
            f_pb = 1.0;
            f_at = 1.0;
            f_bt = 1.0;
            f_c = 0.1;
            xvmax = 0.01;
            rho = 890;
            overlap_pa = -1e-6;
            overlap_pb = -1e-6;
            overlap_at = -1e-6;
            overlap_bt = -1e-6;
            omegah = 100.0;
            deltah = 1.0;

            mpPP = addPowerPort("PP", "NodeHydraulic");
            mpPT = addPowerPort("PT", "NodeHydraulic");
            mpPA = addPowerPort("PA", "NodeHydraulic");
            mpPB = addPowerPort("PB", "NodeHydraulic");
            mpIn = addReadPort("in", "NodeSignal");
            mpOut = addWritePort("xv", "NodeSignal", Port::NotRequired);

            registerParameter("C_q", "Flow Coefficient", "[-]", Cq);
            registerParameter("rho", "Oil Density", "[kg/m^3]", rho);
            registerParameter("d", "Spool Diameter", "[m]", d);
            registerParameter("x_vmax", "Maximum Spool Displacement", "[m]", xvmax);
            registerParameter("p_c",  "Fraction of displacement when central position is open", "[-]", p_c);
            registerParameter("f_pa", "Fraction of spool circumference opening P-A", "[-]", f_pa);
            registerParameter("f_pb", "Fraction of spool circumference opening P-B", "[-]", f_pb);
            registerParameter("f_at", "Fraction of spool circumference opening A-T", "[-]", f_at);
            registerParameter("f_bt", "Fraction of spool circumference opening B-T", "[-]", f_bt);
            registerParameter("f_c",  "Fraction of spool circumference opening at neutral position", "-", f_c);
            registerParameter("x_pa", "Spool Overlap From Port P To A", "[m]", overlap_pa);
            registerParameter("x_pb", "Spool Overlap From Port P To B", "[m]", overlap_pb);
            registerParameter("x_at", "Spool Overlap From Port A To T", "[m]", overlap_at);
            registerParameter("x_bt", "Spool Overlap From Port B To T", "[m]", overlap_bt);
            registerParameter("omega_h", "Resonance Frequency", "[rad/s]", omegah);
            registerParameter("delta_h", "Damping Factor", "[-]", deltah);
        }


        void initialize()
        {
            mpND_pp = getSafeNodeDataPtr(mpPP, NodeHydraulic::PRESSURE);
            mpND_qp = getSafeNodeDataPtr(mpPP, NodeHydraulic::FLOW);
            mpND_cp = getSafeNodeDataPtr(mpPP, NodeHydraulic::WAVEVARIABLE);
            mpND_Zcp = getSafeNodeDataPtr(mpPP, NodeHydraulic::CHARIMP);

            mpND_pt = getSafeNodeDataPtr(mpPT, NodeHydraulic::PRESSURE);
            mpND_qt = getSafeNodeDataPtr(mpPT, NodeHydraulic::FLOW);
            mpND_ct = getSafeNodeDataPtr(mpPT, NodeHydraulic::WAVEVARIABLE);
            ZmpND_ct = getSafeNodeDataPtr(mpPT, NodeHydraulic::CHARIMP);

            mpND_pa = getSafeNodeDataPtr(mpPA, NodeHydraulic::PRESSURE);
            mpND_qa = getSafeNodeDataPtr(mpPA, NodeHydraulic::FLOW);
            mpND_ca = getSafeNodeDataPtr(mpPA, NodeHydraulic::WAVEVARIABLE);
            mpND_Zca = getSafeNodeDataPtr(mpPA, NodeHydraulic::CHARIMP);

            mpND_pb = getSafeNodeDataPtr(mpPB, NodeHydraulic::PRESSURE);
            mpND_qb = getSafeNodeDataPtr(mpPB, NodeHydraulic::FLOW);
            mpND_cb = getSafeNodeDataPtr(mpPB, NodeHydraulic::WAVEVARIABLE);
            mpND_Zcb = getSafeNodeDataPtr(mpPB, NodeHydraulic::CHARIMP);

            mpND_xvin = getSafeNodeDataPtr(mpIn, NodeSignal::VALUE);
            mpND_xvout = getSafeNodeDataPtr(mpOut, NodeSignal::VALUE);

            double num[3] = {1.0, 0.0, 0.0};
            double den[3] = {1.0, 2.0*deltah/omegah, 1.0/(omegah*omegah)};
            filter.initialize(mTimestep, num, den, 0, 0, -xvmax, xvmax);
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double cp, Zcp, ct, Zct, ca, Zca, cb, Zcb, xvin, xv, xpanom, xpbnom, xatnom, xbtnom, xcnom, Kcpa, Kcpb, Kcat, Kcbt, Kcc, qpa, qpb, qat, qbt, qp, qa, qb, qt, pa, pb, pt, pp;
            bool cav = false;

            //Get variable values from nodes
            cp = (*mpND_cp);
            Zcp = (*mpND_Zcp);
            ct  = (*mpND_ct);
            Zct = (*ZmpND_ct);
            ca  = (*mpND_ca);
            Zca = (*mpND_Zca);
            cb  = (*mpND_cb);
            Zcb = (*mpND_Zcb);
            xvin  = (*mpND_xvin);

            limitValue(xvin, -xvmax, xvmax);
            filter.update(xvin);
            xv = filter.value();

            //Valve equations
            xpanom = std::max(xv-overlap_pa,0.0);
            xpbnom = std::max(-xv-overlap_pb,0.0);
            xatnom = std::max(-xv-overlap_at,0.0);
            xbtnom = std::max(xv-overlap_bt,0.0);
            xcnom  = std::max(xvmax - fabs(xv)/(p_c), 0.0);

            Kcpa = Cq*f_pa*pi*d*xpanom*sqrt(2.0/rho);
            Kcpb = Cq*f_pb*pi*d*xpbnom*sqrt(2.0/rho);
            Kcat = Cq*f_at*pi*d*xatnom*sqrt(2.0/rho);
            Kcbt = Cq*f_bt*pi*d*xbtnom*sqrt(2.0/rho);
            Kcc  = Cq*f_c*pi*d*xcnom*sqrt(2.0/rho);

            //With TurbulentFlowFunction:
            qTurb_pa.setFlowCoefficient(Kcpa);
            qTurb_pb.setFlowCoefficient(Kcpb);
            qTurb_at.setFlowCoefficient(Kcat+Kcc);
            qTurb_bt.setFlowCoefficient(Kcbt+Kcc);

            qpa = qTurb_pa.getFlow(cp, ca, Zcp, Zca);
            qpb = qTurb_pb.getFlow(cp, cb, Zcp, Zcb);
            qat = qTurb_at.getFlow(ca, ct, Zca, Zct);
            qbt = qTurb_bt.getFlow(cb, ct, Zcb, Zct);

            qp = -qpa-qpb;
            qa = qpa-qat;
            qb = -qbt+qpb;
            qt = qbt+qat;

            pp = cp + qp*Zcp;
            pt = ct + qt*Zct;
            pa = ca + qa*Zca;
            pb = cb + qb*Zcb;

            //Cavitation check
            if(pa < 0.0)
            {
                ca = 0.0;
                Zca = 0;
                cav = true;
            }
            if(pb < 0.0)
            {
                cb = 0.0;
                Zcb = 0;
                cav = true;
            }
            if(pp < 0.0)
            {
                cp = 0.0;
                Zcp = 0;
                cav = true;
            }
            if(pt < 0.0)
            {
                ct = 0.0;
                Zct = 0;
                cav = true;
            }

            if(cav)
            {
                qpa = qTurb_pa.getFlow(cp, ca, Zcp, Zca);
                qpb = qTurb_pb.getFlow(cp, cb, Zcp, Zcb);
                qat = qTurb_at.getFlow(ca, ct, Zca, Zct);
                qbt = qTurb_bt.getFlow(cb, ct, Zcb, Zct);

                qp = -qpa-qpb;
                qa = qpa-qat;
                qb = -qbt+qpb;
                qt = qbt+qat;

                pp = cp + qp*Zcp;
                pt = ct + qt*Zct;
                pa = ca + qa*Zca;
                pb = cb + qb*Zcb;
            }

            //Write new values to nodes

            (*mpND_pp) = cp + qp*Zcp;
            (*mpND_qp) = qp;
            (*mpND_pt) = ct + qt*Zct;
            (*mpND_qt) = qt;
            (*mpND_pa) = ca + qa*Zca;
            (*mpND_qa) = qa;
            (*mpND_pb) = cb + qb*Zcb;
            (*mpND_qb) = qb;
            (*mpND_xvout) = xv;
        }
    };
}

#endif // HYDRAULIC43ValveNeutralToTank_HPP_INCLUDED

