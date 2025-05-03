#include <gtest/gtest.h>
#include "../TaskList.h"

TEST(TestTaskList, TaskListAddTask_BothVersion) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    ASSERT_EQ(taskList.getNextId(), 1);

    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(7, "titolo2", "descrizione2", 1, true);

    taskList.addTask("titolo1", "descrizione1", 1);

    Task task = taskList.getTask(0);
    ASSERT_TRUE(task.getId() == task1.getId());
    ASSERT_TRUE(task.getUrgencyLevel() == task1.getUrgencyLevel());
    ASSERT_TRUE(task.isCompleted() == task1.isCompleted());
    ASSERT_TRUE(task.getTitle() == task1.getTitle());
    ASSERT_TRUE(task.getDescription() == task1.getDescription());
    ASSERT_EQ(taskList.getNextId(), 2);

    taskList.addTask(task2);
    task = taskList.getTask(1);
    ASSERT_TRUE(task2.getId() == task.getId());
    ASSERT_TRUE(task2.getUrgencyLevel() == task.getUrgencyLevel());
    ASSERT_TRUE(task2.isCompleted() == task.isCompleted());
    ASSERT_TRUE(task2.getTitle() == task.getTitle());
    ASSERT_TRUE(task2.getDescription() == task.getDescription());
    ASSERT_EQ(taskList.getNextId(), 8);

    taskList.addTask(task2);
    task = taskList.getTask(2);
    ASSERT_TRUE(task2.getId() == (taskList.getNextId()-1));
    ASSERT_TRUE(task2.getUrgencyLevel() == task.getUrgencyLevel());
    ASSERT_TRUE(task2.isCompleted() == task.isCompleted());
    ASSERT_TRUE(task2.getTitle() == task.getTitle());
    ASSERT_TRUE(task2.getDescription() == task.getDescription());
    ASSERT_EQ(taskList.getNextId(), 9);
}

TEST(TestTaskList, TaskListGetXXXurgencyLevelTaskPosition) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    string titolo = "titolo";
    string descrizione = "descrizione";
    taskList.addTask(titolo, descrizione, 0);
    taskList.addTask(titolo, descrizione, 0);
    taskList.addTask(titolo, descrizione, 2);
    taskList.addTask(titolo, descrizione, 3);
    taskList.addTask(titolo, descrizione, 1);
    taskList.addTask(titolo, descrizione, 0);

    vector<int> positions;
    taskList.getXXXurgencyLevelTaskPosition(0, positions);
    ASSERT_EQ(positions[0], 0);
    ASSERT_EQ(positions[1], 1);
    ASSERT_EQ(positions[2], 5);

    taskList.getXXXurgencyLevelTaskPosition(1, positions);
    ASSERT_EQ(positions[0], 4);

    taskList.getXXXurgencyLevelTaskPosition(2, positions);
    ASSERT_EQ(positions[0], 2);

    taskList.getXXXurgencyLevelTaskPosition(3, positions);
    ASSERT_EQ(positions[0], 3);
}

TEST(TestTaskList, TaskListToString) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    string result;
    taskList.toString(result);
    EXPECT_EQ(result, "");
    string titolo = "titolo";
    string descrizione = "descrizione";
    taskList.addTask(titolo+"1", descrizione, 0);
    taskList.addTask(titolo+"2", descrizione, 0);
    taskList.addTask(titolo+"3", descrizione, 1);
    result = "";
    taskList.toString(result);
    EXPECT_EQ(result, "Task di urgenza Medio: \nTaskID: 3 | [ ] - titolo3 - \ndescrizione\n\nTask di urgenza Basso: \nTaskID: 1 | [ ] - titolo1 - \ndescrizione\n\nTaskID: 2 | [ ] - titolo2 - \ndescrizione\n\n");
}

TEST(TestTaskList, TaskListSerialize) {
    string name = "nome";
    TaskList taskList(0, name);
    taskList.setNextId(1);
    string titolo = "titolo";
    string descrizione = "descrizione";
    taskList.addTask(titolo+"1", descrizione, 0);
    taskList.addTask(titolo+"2", descrizione, 1);
    taskList.addTask(titolo+"3", descrizione, 2);
    vector<string> result;
    taskList.serialize(result);
    EXPECT_EQ(result[0], "0|nome");
    EXPECT_EQ(result[1], "1|0|0|titolo1|descrizione|");
    EXPECT_EQ(result[2], "2|1|0|titolo2|descrizione|");
    EXPECT_EQ(result[3], "3|2|0|titolo3|descrizione|");
};

TEST(TestTaskList, TaskListDeserialize) {
    string name = "sdf";
    TaskList taskList(0, name);
    taskList.setNextId(1);
    vector<string> result = {
        "0|nome",
        "1|0|0|titolo1|descrizione|",
        "2|1|0|titolo2|descrizione|",
        "3|2|0|titolo3|descrizione|"
    };
    taskList.deserialize(result);
    ASSERT_EQ(taskList.getTaskListID(), 0);
    EXPECT_EQ(taskList.getName(), "nome");

    Task task = taskList.getTask(0);
    ASSERT_TRUE(task.getId() == 1);
    ASSERT_TRUE(task.getUrgencyLevel() == 0);
    ASSERT_TRUE(task.isCompleted() == 0);
    ASSERT_TRUE(task.getTitle() == "titolo1");
    ASSERT_TRUE(task.getDescription() == "descrizione");

    task = taskList.getTask(1);
    ASSERT_TRUE(task.getId() == 2);
    ASSERT_TRUE(task.getUrgencyLevel() == 1);
    ASSERT_TRUE(task.isCompleted() == 0);
    ASSERT_TRUE(task.getTitle() == "titolo2");
    ASSERT_TRUE(task.getDescription() == "descrizione");

    task = taskList.getTask(2);
    ASSERT_TRUE(task.getId() == 3);
    ASSERT_TRUE(task.getUrgencyLevel() == 2);
    ASSERT_TRUE(task.isCompleted() == 0);
    ASSERT_TRUE(task.getTitle() == "titolo3");
    ASSERT_TRUE(task.getDescription() == "descrizione");
}

TEST(TestTaskList, TaskListCompleteTask) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    string titolo = "titolo";
    string descrizione = "descrizione";
    taskList.addTask(titolo+"1", descrizione, 0);
    taskList.addTask(titolo+"2", descrizione, 0);
    taskList.addTask(titolo+"3", descrizione, 1);
    taskList.completeTask(1);
    taskList.completeTask(2);
    ASSERT_TRUE(taskList.getTask(0).isCompleted());
    ASSERT_TRUE(taskList.getTask(1).isCompleted());
    ASSERT_FALSE(taskList.getTask(2).isCompleted());
}

TEST(TestTaskList, TaskListDeleteTask) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    ASSERT_EQ(taskList.getNextId(), 1);
    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(7, "titolo2", "descrizione2", 1, true);
    taskList.addTask("titolo3", "descrizione3", 1);
    taskList.addTask(task1);
    taskList.addTask(task2);
    ASSERT_EQ(taskList.getNumberOfTask(), 3);
    ASSERT_TRUE(taskList.deleteTask(1));
    ASSERT_EQ(taskList.getNumberOfTask(), 2);
    ASSERT_FALSE(taskList.deleteTask(1));
    ASSERT_EQ(taskList.getNumberOfTask(), 2);
}

TEST(TestTaskList, TaskListModifyTask) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    ASSERT_EQ(taskList.getNextId(), 1);
    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(7, "titolo2", "descrizione2", 1, true);
    taskList.addTask("titolo3", "descrizione3", 1);
    taskList.addTask(task1);
    taskList.addTask(task2);

    ASSERT_EQ(taskList.getTask(0).getId(), 1);
    EXPECT_EQ(taskList.getTask(0).getTitle(), "titolo3");
    EXPECT_EQ(taskList.getTask(0).getDescription(), "descrizione3");
    ASSERT_EQ(taskList.getTask(0).getUrgencyLevel(), 1);
    ASSERT_FALSE(taskList.getTask(0).isCompleted());
    string titolo = "newTitle", descrizione = "newDescription";
    taskList.modifyTask(1, titolo, descrizione, 0, true);
    ASSERT_EQ(taskList.getTask(2).getId(), 1);
    EXPECT_EQ(taskList.getTask(2).getTitle(), titolo);
    EXPECT_EQ(taskList.getTask(2).getDescription(), descrizione);
    ASSERT_EQ(taskList.getTask(2).getUrgencyLevel(), 0);
    ASSERT_TRUE(taskList.getTask(2).isCompleted());
    ASSERT_FALSE(taskList.modifyTask(66, titolo, descrizione, 0, true));
}

TEST(TestTaskList, TaskListTaskContains) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    Task task1(1, "Titolo_Task1", "Descrizione_Task1", 2);
    Task task2(2, "Titolo_Task2", "Descrizione_Task2", 1, true);
    Task task3(3, "Titolo_Task4", "Descrizione_Task3", 0, true);
    Task task4(4, "Titolo_Task5", "Descrizione_Task4", 3, true);
    taskList.addTask(task1);
    taskList.addTask(task2);
    taskList.addTask(task3);
    taskList.addTask(task4);

    vector<int> taskIDs;
    string word = "Task";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 4);
    word = "1";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 1);
    word = "Descrizione";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 4);
    word = "Basso";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 3);
    word = "Medio";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 2);
    word = "Alto";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 1);
    word = "Critico";
    taskList.taskContains(word, taskIDs);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 4);
}

TEST(TestTaskList, TaskListGetNumberOfNotCompletedTask) {
    string name;
    TaskList taskList(0, name);
    taskList.setNextId(1);
    Task task1(1, "Titolo_Task1", "Descrizione_Task1", 2);
    Task task2(2, "Titolo_Task2", "Descrizione_Task2", 1, true);
    Task task3(3, "Titolo_Task4", "Descrizione_Task3", 0, true);
    Task task4(4, "Titolo_Task5", "Descrizione_Task4", 3, true);
    taskList.addTask(task1);
    taskList.addTask(task2);
    taskList.addTask(task3);
    taskList.addTask(task4);
    ASSERT_EQ(taskList.getNumberOfNotCompletedTask(), 1);
    taskList.addTask(task1);
    ASSERT_EQ(taskList.getNumberOfNotCompletedTask(), 2);
    taskList.addTask(task1);
    taskList.addTask(task1);
    ASSERT_EQ(taskList.getNumberOfNotCompletedTask(), 4);
    taskList.addTask(task2);
    ASSERT_EQ(taskList.getNumberOfNotCompletedTask(), 4);
}