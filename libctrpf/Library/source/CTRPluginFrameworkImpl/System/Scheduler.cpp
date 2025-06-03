#include "CTRPluginFrameworkImpl/System/Scheduler.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"

namespace CTRPluginFramework
{
#define SCH_DEBUG 0

    Scheduler   Scheduler::_singleton;
    void Scheduler__CoreHandler(void *arg);

    Scheduler::Core::Core(void) :
        thread(Scheduler__CoreHandler, 0x1000, 1 , 1)
    {
        LightEvent_Init(&newTaskEvent, RESET_STICKY);
    }

    void    Scheduler::Core::Assign(const Task &task)
    {
        if (state != Core::Idle)
            return;

        state = Core::Busy;
        taskCtx = task.context;
        taskCtx->flags = Task::Scheduled;
        LightEvent_Signal(&newTaskEvent);
    }

    void     Scheduler__CoreHandler(void *arg)
    {
        Scheduler::Core &core = *reinterpret_cast<Scheduler::Core *>(arg);

        core.tlsPtr = getThreadLocalStorage();

        while (core.state != Scheduler::Core::Exit)
        {
            // Reset core state
            core.state = Scheduler::Core::Idle;

            // Fetch a task from the queue
            Scheduler::_PollTask(core);

            if (core.taskCtx == nullptr)
            {
                LightEvent_Wait(&core.newTaskEvent);
                LightEvent_Clear(&core.newTaskEvent);
            }

            if (SystemImpl::Status())
                return;
#if SCH_DEBUG
            OSD::Notify(Utils::Format("New task on core: %d", core.id));
#endif
            core.state = Scheduler::Core::Busy;

            auto&       taskCtx = core.taskCtx;
            TaskContext *ctx = taskCtx.get();

            if (ctx != nullptr && ctx->func != nullptr)
            {
                ctx->flags = Task::Processing;

                ctx->result = ctx->func(ctx->arg);
            }

            // Signal the task is done if the TaskContext isn't detached
            if (taskCtx.use_count() > 1)
            {
                ctx->flags = Task::Finished;
                LightEvent_Signal(&ctx->event);
            }

            // Release Core::taskContext ownership
            taskCtx.reset();
#if SCH_DEBUG
            OSD::Notify(Utils::Format("Task ended on core: %d", core.id));
#endif
        }
    }

    int     Scheduler::Schedule(const Task& task)
    {
        Lock    lock(_singleton._mutex);
        Core    *cores = _singleton._cores;

        if (task.context == nullptr)
            return -1;

        TaskContext *ctx = task.context.get();

        if (SystemImpl::IsNew3DS && ctx->affinity > AllCores)
            ctx->affinity = AllCores;

        if (!SystemImpl::IsNew3DS && ctx->affinity > OldCores)
            ctx->affinity = OldCores;

        if (ctx->affinity == -1)
            ctx->affinity = AllCores;

        // Ensure memory is updated
        if (!SystemImpl::IsCitra) svcFlushProcessDataCache(Process::GetHandle(), (u32)ctx, sizeof(TaskContext));
        if (ctx->arg)
            if (!SystemImpl::IsCitra) svcFlushProcessDataCache(Process::GetHandle(), (u32)ctx->arg, 0x1000);

        // Search for an idle core matching the Task affinity
        // NewAppCore -> AppCore (0x21) -> AppCore (0x20) -> SysCore
        for (s32 i = 3; i >= 0; --i)
        {
            if (cores[i].state == Core::Idle && cores[i].id & ctx->affinity)
            {
                cores[i].Assign(task);
                return 0;
            }
        }

        // Enqueue the task
        ctx->flags = Task::Scheduled;
        _singleton._tasks.push_back(task.context);

        return 0;
    }

    void    Scheduler::Initialize(void)
    {
        Core *_cores = _singleton._cores;

        // Create handler on Core0
        _cores[1].id = AppCore;
        _cores[1].thread.affinity = 0;
        _cores[1].thread.priority = 0x20;
        _cores[1].thread.Start(&_cores[1]);

        // Create another handler on Core0
        _cores[2].id = AppCore;
        _cores[2].thread.affinity = 0;
        _cores[2].thread.priority = 0x21;
        _cores[2].thread.Start(&_cores[2]);

        // Create handler on Core1
        _cores[0].id = SysCore;
        _cores[0].thread.affinity = 1;
        _cores[0].thread.priority = 0x20;
        _cores[0].thread.Start(&_cores[0]);

        // Create handler on Core2 (N3DS only)
        if (!System::IsNew3DS())
        {
            _cores[3].state = Core::Exit;
        }
        else
        {
            _cores[3].id = NewAppCore;
            _cores[3].thread.affinity = 2;
            _cores[3].thread.priority = 0x18;
            _cores[3].thread.Start(&_cores[3]);
        }
    }

    void    Scheduler::Exit(void)
    {
        Lock lock(_singleton._mutex);

        for (Core& core : _singleton._cores)
        {
            core.state = Core::Exit;
            LightEvent_Signal(&core.newTaskEvent);
        }
    }

    bool    Scheduler::CurrentThreadIsTaskHandler(void)
    {
        void *current = getThreadLocalStorage();

        for (const Core& core : _singleton._cores)
            if (core.tlsPtr == current)
                return true;

        return false;
    }

    // Function only called from a Scheduler::Core obj
    void    Scheduler::_PollTask(Core& core)
    {
        Lock    lock(_singleton._mutex);
        auto&   tasks = _singleton._tasks;

        // Check current core flags after lock is acquired
        if (core.state == Core::Busy)
            return; ///< We were assigned a task by the Scheduler

        // If there are tasks in the queue
        for (auto& task : tasks)
        {
            // If the task's affinity match this core
            if (task->affinity & core.id)
            {
                core.taskCtx = task;
                tasks.erase(std::remove(tasks.begin(), tasks.end(), task), tasks.end());
                return;
            }
        }
    }
}
