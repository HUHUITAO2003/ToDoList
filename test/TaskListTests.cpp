#include <gtest/gtest.h>
#include "../Task.h"
#include "../TaskList.h"

TEST(TestTaskList, TaskListAddTask_BothVersion) {
    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(7, "titolo2", "descrizione2", 1, true);
    TaskList taskList;
    ASSERT_EQ(taskList.getNextId(), 1);

    taskList.addTask("titolo1", "descrizione1", 1);
    Task task = taskList.getTasks().at(0);
    ASSERT_TRUE(task.getId() == task1.getId());
    ASSERT_TRUE(task.getUrgencyLevel() == task1.getUrgencyLevel());
    ASSERT_TRUE(task.isCompleted() == task1.isCompleted());
    ASSERT_TRUE(task.getTitle() == task1.getTitle());
    ASSERT_TRUE(task.getDescription() == task1.getDescription());
    ASSERT_EQ(taskList.getNextId(), 2);

    taskList.addTask(task2);
    task = taskList.getTasks().at(1);
    ASSERT_TRUE(task2.getId() == task.getId());
    ASSERT_TRUE(task2.getUrgencyLevel() == task.getUrgencyLevel());
    ASSERT_TRUE(task2.isCompleted() == task.isCompleted());
    ASSERT_TRUE(task2.getTitle() == task.getTitle());
    ASSERT_TRUE(task2.getDescription() == task.getDescription());
    ASSERT_EQ(taskList.getNextId(), 8);

    taskList.addTask(task2);
    task = taskList.getTasks().at(2);
    ASSERT_TRUE(task2.getId() == (taskList.getNextId()-1));
    ASSERT_TRUE(task2.getUrgencyLevel() == task.getUrgencyLevel());
    ASSERT_TRUE(task2.isCompleted() == task.isCompleted());
    ASSERT_TRUE(task2.getTitle() == task.getTitle());
    ASSERT_TRUE(task2.getDescription() == task.getDescription());
    ASSERT_EQ(taskList.getNextId(), 9);
}

TEST(TestTaskList, TaskListSaveAndLoad) {
    TaskList taskList;
    taskList.setPath("./test/testToDo.txt");
    ASSERT_TRUE(taskList.load());

    Task task1(200, "titolo1", "descrizione1", 0);
    taskList.addTask(task1);
    Task task = taskList.getTasks().at(0);
    ASSERT_TRUE(task1.getId() == (taskList.getNextId()-1));
    ASSERT_TRUE(task1.getUrgencyLevel() == task.getUrgencyLevel());
    ASSERT_TRUE(task1.isCompleted() == task.isCompleted());
    ASSERT_TRUE(task1.getTitle() == task.getTitle());
    ASSERT_TRUE(task1.getDescription() == task.getDescription());
    ASSERT_EQ(taskList.getNextId(), 201);

    taskList.addTask("SALVATO", "SALVATO", 0);
    taskList.save();

    TaskList taskList2;
    taskList2.setPath("./test/testToDo.txt");
    taskList2.load();
    const Task &task2 = taskList2.getTasks().back();

    ASSERT_TRUE(task2.getId() == (taskList2.getNextId()-1));
    ASSERT_TRUE(task2.getUrgencyLevel() == 0);
    ASSERT_TRUE(task2.isCompleted() == false);
    ASSERT_TRUE(task2.getTitle() == "SALVATO");
    ASSERT_TRUE(task2.getDescription() == "SALVATO");
    ASSERT_EQ(taskList.getNextId(), 202);

    ofstream file("./test/testToDo.txt");
    file << "1|0|0|titolo1|descrizione1\n2|1|1|titolo2|descrizione2\n3|2|0|titolo3|descrizione3\n4|3|1|titolo4|descrizione4";
    file.close();
}