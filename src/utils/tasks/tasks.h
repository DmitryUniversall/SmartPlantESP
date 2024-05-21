#pragma once

#include <Arduino.h>
#include <vector>


namespace Tasks {
    using TaskCallback = std::function<void(void)>;

    enum TaskState {
        IDLING,
        WAITING,
        PROCESSING,
        DONE
    };

    class Task {
    public:
        Task(ulong callAt, TaskCallback callback);

        virtual ~Task() = default;

        virtual bool deleteAfterProcess() const;

        void callSoon();

        virtual void process();

        void cancel(bool del = true);

        TaskState getState() const;

        void setState(TaskState state);

        ulong getCallAt() const;

        uint getTaskID() const;

    protected:
        static uint _last_task_id;

        uint _taskID;
        ulong _callAt;
        TaskState _state;
        TaskCallback _callback;
    };

    class LoopedTask : public Task {
    public:
        LoopedTask(ulong callAt, ulong interval, TaskCallback callback);

        LoopedTask(ulong interval, TaskCallback callback);

        void process() override;

        bool deleteAfterProcess() const override;

    protected:
        ulong _interval;
    };

    class TaskManager {
    public:
        ~TaskManager();

        void update();

        void callSoon(Task* task);

        void cancelTask(uint taskID, bool del = true);

        void cancelTask(Task* task, bool del = true);

        Task* callAfter(ulong after, const TaskCallback& callback);

        LoopedTask* loop(ulong after, ulong period, const TaskCallback& callback);

    private:
        std::vector<Task*> _tasks;
    };
}

extern Tasks::TaskManager taskManager;
