#include "CTRPluginFramework/System/Task.hpp"
#include "CTRPluginFrameworkImpl/System/Scheduler.hpp"

namespace CTRPluginFramework
{
    Task::Task(TaskFunc func, void *arg, s32 affinity)
    {
        context = std::make_shared<TaskContext>();

        context->affinity = affinity;
        context->func = func;
        context->arg = arg;
        LightEvent_Init(&context->event, RESET_STICKY);
    }

    Task::Task(const Task& task)
    {
        context = task.context;
    }

    Task::Task(Task&& task) noexcept
    {
        context = std::move(task.context);
    }

    Task&   Task::operator=(const Task& right)
    {
        context = right.context;
        return *this;
    }

    Task&   Task::operator=(Task&& right) noexcept
    {
        context = std::move(right.context);
        return *this;
    }

    int     Task::Start(void) const
    {
        if (context == nullptr || context->flags & (Scheduled | Processing))
            return -1;

        LightEvent_Clear(&context->event);
        return Scheduler::Schedule(*this);
    }

    int     Task::Start(void *arg) const
    {
        if (context == nullptr || context->flags & (Scheduled | Processing))
            return -1;

        context->arg = arg;
        LightEvent_Clear(&context->event);
        return Scheduler::Schedule(*this);
    }

    s32     Task::Wait(void) const
    {
        if (context == nullptr)
            return -1;

        if (context->flags == Task::Finished || context->flags == Task::Idle)
            return context->result;

        LightEvent_Wait(&context->event);
        return context->result;
    }

    u32     Task::Status(void) const
    {
        if (context == nullptr)
            return -1;

        return context->flags;
    }
}
