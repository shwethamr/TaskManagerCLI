// TaskManagerCLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>
#include<string>
#include<optional>
#include<algorithm>
using namespace std;
enum class TaskStatus
{
    Pending,
    Done
};
string to_string(TaskStatus status)
{
    return status == TaskStatus::Pending ? "Pending" : "Done";
}
TaskStatus from_string(const string& str)
{
    return str == "Pending" ? TaskStatus::Pending : TaskStatus::Done;
}

struct Task
{
    string description;
    TaskStatus status = TaskStatus::Pending;
    string serialize() const
    {
        return to_string(status) + "|" + description;
    }
    static std::optional<Task> deserialize(const string& line)
    {
        auto delim_pos = line.find('|');
        if (delim_pos == std::string::npos) return std::nullopt;
        Task task;
        task.status = from_string(line.substr(0, delim_pos));
        task.description = line.substr(delim_pos + 1);
        return task;
    }

};
class TaskManager {
    std::vector<std::unique_ptr<Task>> tasks;
public:
    void add_task(const string& desc)
    {
        tasks.push_back(make_unique<Task>(Task{ desc }));
    }
    void remove_task(size_t index)
    {
        if (index < 1 || index > tasks.size())
        {
            cout << "Invalid task number \n";
            return;
        }
        tasks.erase(tasks.begin() + (index - 1));
    }
    void list_tasks() const
    {
        if (tasks.empty())
        {
            std::cout << "No tasks avaiable \n";
            return;
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << (i + 1) << ". [" << to_string(tasks[i]->status) << "] "
                << tasks[i]->description << "\n";
        }
    }
    void mark_done(size_t index) {
        if (index < 1 || index > tasks.size()) {
            std::cout << "Invalid task number.\n";
            return;
        }
        tasks[index - 1]->status = TaskStatus::Done;
    }
    void save(const std::string& filename) const {
        std::ofstream file(filename);
        for (const auto& task : tasks) {
            file << task->serialize() << "\n";
        }
        std::cout << "Tasks saved.\n";
    }

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return;

        tasks.clear();
        std::string line;
        while (std::getline(file, line)) {
            auto task = Task::deserialize(line);
            if (task) {
                tasks.push_back(std::make_unique<Task>(*task));
            }
        }
        std::cout << "Tasks loaded.\n";
    }
};
void show_menu() {
    std::cout << "\n=== Task Manager ===\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. Remove Task\n";
    std::cout << "3. List Tasks\n";
    std::cout << "4. Mark Task as Done\n";
    std::cout << "5. Save\n";
    std::cout << "6. Load\n";
    std::cout << "0. Exit\n";
    std::cout << "Select: ";
}
int main()
{
    TaskManager manager;
    manager.load("tasks.txt");

    while (true) {
        show_menu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // flush newline

        switch (choice) {
        case 1: {
            std::string desc;
            std::cout << "Enter task description: ";
            std::getline(std::cin, desc);
            manager.add_task(desc);
            break;
        }
        case 2: {
            size_t index;
            std::cout << "Enter task number to remove: ";
            std::cin >> index;
            manager.remove_task(index);
            break;
        }
        case 3:
            manager.list_tasks();
            break;
        case 4: {
            size_t index;
            std::cout << "Enter task number to mark done: ";
            std::cin >> index;
            manager.mark_done(index);
            break;
        }
        case 5:
            manager.save("tasks.txt");
            break;
        case 6:
            manager.load("tasks.txt");
            break;
        case 0:
            std::cout << "Goodbye!\n";
            return 0;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
