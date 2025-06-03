#ifndef CTRPLUGINFRAMEWORK_SYSTEM_TASK_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_TASK_HPP

#include "types.h"
#include "3ds.h"
#include <atomic>
#include <memory>

namespace CTRPluginFramework
{
    using TaskFunc = s32 (*)(void *);

    struct TaskContext
    {
        std::atomic<u32>    flags{0};
        s32                 affinity{-1};
        s32                 result{0};
        void *              arg{nullptr};
        TaskFunc            func{nullptr};
        LightEvent          event{};
    };

    using TaskContextPtr = std::shared_ptr<TaskContext>;
    struct Task
    {
        enum Status
        {
            Idle = 0,
            Scheduled = 1,
            Processing = 2,
            Finished = 4
        };

        enum Affinity
        {
            AppCore = 1 << 0,
            SysCore = 1 << 1,
            NewAppCore = 1 << 2,
            // Support for NewSysCore disabled, as spawning threads there
            // can slow down the system. (Heavily used by 3D head tracking code.)
            // NewSysCore = 1 << 3,

            AppCores = AppCore | NewAppCore,
            SysCores = SysCore, // | NewSysCore
            AllCores = AppCores | SysCores
        };

        TaskContextPtr    context;

        explicit Task(TaskFunc func, void *arg = nullptr, s32 affinity = -1);
        Task(const Task& task);
        Task(Task&& task) noexcept;
        ~Task(void) = default;

        Task&   operator=(const Task& right);
        Task&   operator=(Task&& right) noexcept;

        /**
         * \brief Schedule a Task and starts it
         * \return 0 on operation success, -1 if the task is already running (check Status)
         */
        int     Start(void) const;
        int     Start(void *arg) const;

        /**
         * \brief Wait for the Task to be completed
         * \return The result of the Task (returned by TaskFunc)
         */
        s32     Wait(void) const;

        /**
         * \brief Get the current status of the Task
         * \return Task status (see enum)
         */
        u32     Status(void) const;
    };
}

#endif
