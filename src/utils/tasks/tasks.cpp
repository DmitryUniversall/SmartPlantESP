#include "tasks.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>


namespace Tasks {  // TODO: Refactor?
    uint Task::_last_task_id = 0;

    Task::Task(ulong callAt, Tasks::TaskCallback callback) :
        _taskID(_last_task_id++),
        _callAt(callAt),
        _callback(std::move(callback)),
        _state(IDLING) {}

    uint Task::getTaskID() const {
        return _taskID;
    }

    ulong Task::getCallAt() const {
        return _callAt;
    }

    TaskState Task::getState() const {
        return _state;
    }

    void Task::setState(TaskState state) {
        _state = state;
    }

    bool Task::deleteAfterProcess() const {
        return true;
    }

    void Task::cancel(bool del) {
        taskManager.cancelTask(this, del);
    }

    void Task::callSoon() {
        taskManager.callSoon(this);
    }

    void Task::process() {
        setState(PROCESSING);
        _callback();
        setState(DONE);
    }

    LoopedTask::LoopedTask(ulong callAt, ulong interval, TaskCallback callback) :
        Task(callAt, std::move(callback)), _interval(interval) {}

    LoopedTask::LoopedTask(ulong interval, Tasks::TaskCallback callback) :
        Task(millis() + interval, std::move(callback)), _interval(interval) {}

    void LoopedTask::process() {
        Task::process();
        _callAt = millis() + _interval;  // Update _callAt to be called again in the future
        setState(WAITING);  // Because task is looped
    }

    bool LoopedTask::deleteAfterProcess() const {  // This task should not be deleted after process
        return false;
    }

    TaskManager::~TaskManager() {
        _tasks.clear();  // Clean up allocated tasks
        _tasks.shrink_to_fit();
    }

    void TaskManager::update() {
        if (_tasks.empty()) return;

        auto it = _tasks.begin();
        while (it != _tasks.end()) {
            Task* const task = *it;  // const pointer to Task

            if (millis() > task->getCallAt()) {
                task->process();

                if (task->deleteAfterProcess()) {  // TODO: Make eraseAfterProcess and deleteAfterProcess
                    delete task;
                    it = _tasks.erase(it);
                    continue;  // Skip incrementing 'it' as erase has already moved it
                }
            }
            ++it;
        }
    }

    void TaskManager::cancelTask(uint task_id, bool del) {
        auto it = std::remove_if(_tasks.begin(), _tasks.end(), [task_id, del](Task* task) {
            if (task->getTaskID() == task_id) {
                task->setState(IDLING);

                if (del) delete task; task = nullptr;
                return true;
            }
            return false;
        });

        _tasks.erase(it, _tasks.end());  // Delete pointer from array
    }

    void TaskManager::cancelTask(Task* task, bool del) {
        task->setState(IDLING);
        cancelTask(task->getTaskID(), del);
    }

    void TaskManager::callSoon(Task* task) {
        task->setState(WAITING);
        _tasks.push_back(task);
    }

    Task* TaskManager::callAfter(ulong after, const Tasks::TaskCallback& callback) {
        auto* task = new Task(millis() + after, callback);
        callSoon(task);
        return task;
    }

    LoopedTask* TaskManager::loop(ulong after, ulong period, const Tasks::TaskCallback& callback) {
        auto* loopedTask = new LoopedTask(millis() + after, period, callback);
        callSoon(loopedTask);
        return loopedTask;
    }
}

Tasks::TaskManager taskManager;
