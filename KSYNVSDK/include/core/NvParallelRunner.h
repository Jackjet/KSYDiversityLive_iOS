//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 13. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <QThread>
#include <QVector>
#include <QWaitCondition>
#include <QMutex>


typedef void (* NvParallelRunnerTaskRoutine)(void *taskParam,
                                             unsigned int threadIdx,
                                             unsigned int totalThreadCount);


class CNvParallelRunnerTaskThread;

class CNvParallelRunner
{
    friend class CNvParallelRunnerTaskThread;

public:
    CNvParallelRunner();
    ~CNvParallelRunner();

public:
    bool Startup(int maxThreadCount = QThread::idealThreadCount(),
                 int stackSize = 0,
                 QThread::Priority priority = QThread::InheritPriority);

    void Shutdown();

    unsigned int WorkerThreadCount() const;
    void ExecuteTask(NvParallelRunnerTaskRoutine taskRoutine, void *taskParam);

private:
    bool m_started;

    struct __TaskInfo {
        CNvParallelRunnerTaskThread *thread;
        volatile bool triggeringTask;
    };

    QVector<__TaskInfo *> m_taskThreadsInfo;

    QWaitCondition m_startTaskCond;
    QWaitCondition m_allTasksFinishedCond;
    volatile unsigned int m_runningThreadCount;
    QMutex m_taskMutex;

    NvParallelRunnerTaskRoutine m_taskRoutine;
    void *m_taskParam;
};

