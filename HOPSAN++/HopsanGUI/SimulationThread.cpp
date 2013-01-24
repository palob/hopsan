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
//! @file   SimulationThread.cpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-03-09
//!
//! @brief Contains a class for simulation in a separate class
//!
//$Id$


#include "SimulationThread.h"
#include "CoreAccess.h"
#include "Configuration.h"
#include "common.h"
#include <QDebug>


//! @class SimulationThread
//! @brief The SimulationThread class implement a class to simulate a core model in a separate thread
//!
//! One reason to implement a simulation in a separate thread is to enable a progress bar.
//!


//! @brief Constructor for simulation thread
//! @param pComponentSystem Pointer to the system to simulate.
//! @param startTime Start time for the simulation.
//! @param finishTime Finish time for the simulation.
//! @param parent Parent of the thread, the ProjectTab (Automatically casted to QObject*)
SimulationThread::SimulationThread(CoreSystemAccess *pGUIRootSystem, double startTime, double finishTime, QObject *parent)
    : QThread(parent)
{
    mpCoreSystemAccess = pGUIRootSystem;

    mStartTime = startTime;
    mFinishTime = finishTime;
}


//! @brief Implements the task for the thread.
void SimulationThread::run()
{
    qDebug() << "Beginning simulate!";

    if(gConfig.getUseMulticore())
    {
        mpCoreSystemAccess->simulate(mStartTime, mFinishTime, gConfig.getNumberOfThreads());
    }
    else
    {
        mpCoreSystemAccess->simulate(mStartTime, mFinishTime, -1);
    }
    //! @todo we should run finalize at some other place even if initialize is aborted
    mpCoreSystemAccess->finalize();

    //exec(); //Is used if one want to run an event loop in this thread.
}


//! @brief Constructor
MultipleSimulationThread::MultipleSimulationThread(QVector<CoreSystemAccess *> vGUIRootSystemPtrs, double startTime, double finishTime, bool modelsHaveNotChanged, QObject *parent)
    : QThread(parent)
{
    mvGUIRootSystemPtrs = vGUIRootSystemPtrs;

    mStartTime = startTime;
    mFinishTime = finishTime;
    mModelsHaveNotChanged = modelsHaveNotChanged;
}


//! @brief Implements the task for the thread.
void MultipleSimulationThread::run()
{
    CoreSimulationHandler simuHandler;
    if(gConfig.getUseMulticore())
    {
        //mvGUIRootSystemPtrs.first()->simulateAllOpenModels(mStartTime, mFinishTime, MULTICORE, gConfig.getNumberOfThreads(), mModelsHaveNotChanged);
        simuHandler.simulate(mStartTime, mFinishTime, gConfig.getNumberOfThreads(), mvGUIRootSystemPtrs, mModelsHaveNotChanged);
    }
    else
    {
        //mvGUIRootSystemPtrs.first()->simulateAllOpenModels(mStartTime, mFinishTime, SINGLECORE, gConfig.getNumberOfThreads(), mModelsHaveNotChanged);
        simuHandler.simulate(mStartTime, mFinishTime, -1, mvGUIRootSystemPtrs, mModelsHaveNotChanged);
    }

    simuHandler.finalize(mvGUIRootSystemPtrs);

//    for(int i=0; i<mvGUIRootSystemPtrs.size(); ++i)
//    {
//        mvGUIRootSystemPtrs.at(i)->finalize();
//    }

    //exec(); //Is used if one want to run an event loop in this thread.
}


CoSimulationThread::CoSimulationThread(CoreSystemAccess *pGUIRootSystem, QObject *parent)
    : QThread(parent)
{
     mpCoreSystemAccess = pGUIRootSystem;
}


//! @brief Implements the task for the thread.
void CoSimulationThread::run()
{
    qDebug() << "Beginning co-simulation!";

    CoreSimulationHandler simuHandler;

    simuHandler.runCoSimulation(mpCoreSystemAccess);

    simuHandler.finalize(mpCoreSystemAccess);

    //exec(); //Is used if one want to run an event loop in this thread.
}
