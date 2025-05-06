//
// Created by huita on 30/04/2025.
//

#include <gtest/gtest.h>
#include "../TaskListManager.h"
#include <filesystem>

TEST(TestTaskListManager, TaskListManagerConstructorAndDestructor) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();


    TaskListManager::setPath(tempFilePath);
    auto taskListManager = new TaskListManager();
    ASSERT_EQ(taskListManager->getNextTaskListID(), 1);
    ASSERT_EQ(taskListManager->getTaskListPosition(0), 0);

    string name = "ciao";
    taskListManager->addTaskList(name);
    taskListManager->addTask(name, name, 0, 1);

    delete taskListManager;

    ifstream readfile(tempFilePath);
    if (!readfile) {
        readfile.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }

    string nextIDsStr;
    getline(readfile, nextIDsStr);
    EXPECT_EQ(nextIDsStr, "2|2|");
    readfile.close();
}

TEST(TestTaskListManager, TaskListManagerAddTaskList) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();


    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;

    string name = "ciao";
    taskListManager.addTaskList(name);
    EXPECT_EQ(taskListManager.getTaskList(0).getName(), "Locale");
    EXPECT_EQ(taskListManager.getTaskList(1).getName(), "ciao");
    TaskList taskList(3, name);
    taskListManager.addTaskList(taskList);
    TaskList gettedTaskList = taskListManager.getTaskList(3);
    ASSERT_EQ(taskList.getTaskListID(), gettedTaskList.getTaskListID());
    EXPECT_EQ(taskList.getName(), gettedTaskList.getName());
    ASSERT_EQ(taskList.getNumberOfTask(), gettedTaskList.getNumberOfTask());
}

TEST(TestTaskListManager, TaskListManagerSavedTaskListToString) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "3|4|\n1:3:1|2:5:2|\n1|ToDoList1|\n1|0|0|Task1|desc1|\n2|ToDoList2|\n2|0|0|Task2|desc2|\n3|0|0|Task3|desc3|\n";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;
    string result = taskListManager.savedTaskListToString();
    EXPECT_EQ(result, "ToDoListID:1 - ToDoList1\nToDoListID:2 - ToDoList2\n");
    std::filesystem::path fullPath = std::filesystem::absolute(tempFilePath);
    cout<<fullPath;
}

TEST(TestTaskListManager, TaskListManagergetTaskListPosition) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;

    string name = "ciao";
    taskListManager.addTaskList(name);
    TaskList taskList1(3, name);
    taskListManager.addTaskList(taskList1);
    TaskList taskList2(99, name);
    taskListManager.addTaskList(taskList2);

    ASSERT_TRUE(taskListManager.getTaskListPosition(0) == 0);
    ASSERT_TRUE(taskListManager.getTaskListPosition(1) == 1);
    ASSERT_TRUE(taskListManager.getTaskListPosition(3) == 2);
    ASSERT_TRUE(taskListManager.getTaskListPosition(99) == 3);
    ASSERT_TRUE(taskListManager.getTaskListPosition(4) == -1);
}

TEST(TestTaskListManager, TaskListManagerGetNumberOfTaskList) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;

    ASSERT_TRUE(taskListManager.getNumberOfTaskList() == 1);

    string name = "ciao";
    taskListManager.addTaskList(name);
    TaskList taskList1(3, name);
    taskListManager.addTaskList(taskList1);
    ASSERT_TRUE(taskListManager.getNumberOfTaskList() == 3);
    TaskList taskList2(99, name);
    taskListManager.addTaskList(taskList2);
    ASSERT_TRUE(taskListManager.getNumberOfTaskList() == 4);
}

TEST(TestTaskListManager, TaskListManagerTaskListToString) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;

    string name = "TaskList", title = "title", description = "description";
    taskListManager.addTaskList(name);
    taskListManager.addTask(title, description,  0, 1);
    taskListManager.addTask(title+"2", description+"2", 0, 1);
    string result = taskListManager.taskListToString(1);
    EXPECT_EQ(result, "Task di urgenza Basso: \nTaskID: 1 | [ ] - title - \ndescription\n\nTaskID: 2 | [ ] - title2 - \ndescription2\n\n");
    taskListManager.addTask(title+"3", description+"3", 1, 1);
    result = taskListManager.taskListToString(1);
    EXPECT_EQ(result, "Task di urgenza Medio: \nTaskID: 3 | [ ] - title3 - \ndescription3\n\nTask di urgenza Basso: \nTaskID: 1 | [ ] - title - \ndescription\n\nTaskID: 2 | [ ] - title2 - \ndescription2\n\n");
}

TEST(TestTaskListManager, TaskListManagerCompleteTask) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;

    string title = "title", description = "description";
    taskListManager.addTask(title, description,  0, 0);
    taskListManager.addTask(title+"2", description+"2", 0, 0);
    ASSERT_FALSE(taskListManager.getTaskList(0).getTask(0).isCompleted());
    taskListManager.completeTask(0,1);
    ASSERT_TRUE(taskListManager.getTaskList(0).getTask(0).isCompleted());
}

TEST(TestTaskListManager, TaskListManagerLoadTaskList) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "3|4|\n1:3:1|2:5:2|\n1|ToDoList1|\n1|0|0|Task1|desc1|\n2|ToDoList2|\n2|0|0|Task2|desc2|\n3|0|0|Task3|desc3|\n";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;
    taskListManager.loadTaskList(1);
    TaskList loadedTaskList = taskListManager.getTaskList(1);
    ASSERT_EQ(loadedTaskList.getTaskListID(), 1);
    EXPECT_EQ(loadedTaskList.getName(), "ToDoList1");
    ASSERT_EQ(loadedTaskList.getNumberOfTask(), 1);
    Task task = loadedTaskList.getTask(0);
    ASSERT_EQ(task.getId(), 1);
    ASSERT_EQ(task.getUrgencyLevel(), 0);
    EXPECT_EQ(task.getTitle(), "Task1");
    EXPECT_EQ(task.getDescription(), "desc1");

    taskListManager.loadTaskList(2);
    loadedTaskList = taskListManager.getTaskList(2);
    ASSERT_EQ(loadedTaskList.getTaskListID(), 2);
    EXPECT_EQ(loadedTaskList.getName(), "ToDoList2");
    ASSERT_EQ(loadedTaskList.getNumberOfTask(), 2);
    task = loadedTaskList.getTask(0);
    ASSERT_EQ(task.getId(), 2);
    ASSERT_EQ(task.getUrgencyLevel(), 0);
    EXPECT_EQ(task.getTitle(), "Task2");
    EXPECT_EQ(task.getDescription(), "desc2");
    task = loadedTaskList.getTask(1);
    ASSERT_EQ(task.getId(), 3);
    ASSERT_EQ(task.getUrgencyLevel(), 0);
    EXPECT_EQ(task.getTitle(), "Task3");
    EXPECT_EQ(task.getDescription(), "desc3");
}

TEST(TestTaskListManager, TaskListManagerSaveTaskList) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;
    string name = "ToDoList1";
    taskListManager.addTaskList(name);
    string title = "Task1", desc = "desc1";
    taskListManager.addTask(title, desc, 0, 1);
    taskListManager.saveTaskList(1);

    vector<string> expectedSavedFile = {"1|1|","1:3:1|","1|ToDoList1","1|0|0|Task1|desc1|"};

    ifstream readFile(tempFilePath);
    if (!readFile) {
        readFile.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    string line;
    for(int i = 0 ; i < expectedSavedFile.size() ; i++) {
        getline(readFile, line);
        EXPECT_EQ(line, expectedSavedFile[i]);
    }
    readFile.close();

    name = "ToDoList2";
    taskListManager.addTaskList(name);
    title = "Task2";
    desc = "desc2";
    taskListManager.addTask(title, desc, 0, 2);
    taskListManager.saveTaskList(2);

    readFile.open(tempFilePath);
    readFile.clear();  // Resetta i flag di errore
    readFile.seekg(0, std::ios::beg);
    expectedSavedFile = {"1|1|","1:3:1|2:5:1|","1|ToDoList1","1|0|0|Task1|desc1|","2|ToDoList2", "2|0|0|Task2|desc2|"};
    for(int i = 0 ; i < expectedSavedFile.size() ; i++) {
        getline(readFile, line);
        EXPECT_EQ(line, expectedSavedFile[i]);
    }
    readFile.close();

    title = "Task3";
    desc = "desc3";
    taskListManager.addTask(title, desc, 0, 2);
    taskListManager.saveTaskList(2);

    readFile.open(tempFilePath);
    readFile.clear();  // Resetta i flag di errore
    readFile.seekg(0, std::ios::beg);
    expectedSavedFile = {"1|1|","1:3:1|2:5:2|","1|ToDoList1","1|0|0|Task1|desc1|","2|ToDoList2", "2|0|0|Task2|desc2|", "3|0|0|Task3|desc3|"};
    for(int i = 0 ; i < expectedSavedFile.size() ; i++) {
        getline(readFile, line);
        EXPECT_EQ(line, expectedSavedFile[i]);
    }
    readFile.close();

    title = "Task4";
    desc = "desc4";
    taskListManager.addTask(title, desc, 0, 1);
    taskListManager.saveTaskList(1);
    readFile.open(tempFilePath);
    readFile.clear();  // Resetta i flag di errore
    readFile.seekg(0, std::ios::beg);
    expectedSavedFile = {"1|1|","1:3:2|2:6:2|","1|ToDoList1","1|0|0|Task1|desc1|", "4|0|0|Task4|desc4|","2|ToDoList2", "2|0|0|Task2|desc2|", "3|0|0|Task3|desc3|"};
    for(int i = 0 ; i < expectedSavedFile.size() ; i++) {
        getline(readFile, line);
        EXPECT_EQ(line, expectedSavedFile[i]);
    }
    readFile.close();
}

TEST(TestTaskListManager, TaskListManagerFindWord) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|\n";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;
    string name;
    taskListManager.addTaskList(name);
    taskListManager.addTaskList(name);
    Task task1(1, "Titolo_Task1", "Descrizione_Task1", 2);
    Task task2(2, "Titolo_Task2", "Descrizione_Task2", 1, true);
    Task task3(3, "Titolo_Task4", "Descrizione_Task3", 0, true);
    Task task4(4, "Titolo_Task5", "Descrizione_Task4", 3, true);
    taskListManager.addTask(task1, 1);
    taskListManager.addTask(task2, 1);
    taskListManager.addTask(task3, 2);
    taskListManager.addTask(task4, 2);

    vector<int> taskIDs;
    string word = "Task";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 4);
    word = "1";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 1);
    word = "Descrizione";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 4);
    word = "Basso";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 3);
    word = "Medio";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 2);
    word = "Alto";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 1);
    word = "Critico";
    taskIDs = taskListManager.findWord(word);
    ASSERT_EQ(taskIDs.size(), 1);
    ASSERT_EQ(taskIDs[0], 4);

    //ricerca parola inesistente
    word = "parola inesistente";
    taskIDs = taskListManager.findWord(word);
    ASSERT_TRUE(taskIDs.empty());
}

TEST(TestTaskListManager, TaskListManagerGetNumberOfNotCompletedTask) {
    string tempFilePath = "./testToDo.txt";
    ofstream file(tempFilePath);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }
    file << "1|1|\n";
    file.close();

    TaskListManager::setPath(tempFilePath);
    TaskListManager taskListManager;
    string name;
    taskListManager.addTaskList(name);
    taskListManager.addTaskList(name);
    Task task1(1, "Titolo_Task1", "Descrizione_Task1", 2);
    Task task2(2, "Titolo_Task2", "Descrizione_Task2", 1, true);
    Task task3(3, "Titolo_Task3", "Descrizione_Task3", 0, true);
    Task task4(4, "Titolo_Task4", "Descrizione_Task4", 3, true);
    taskListManager.addTask(task1, 1);
    taskListManager.addTask(task2, 1);
    taskListManager.addTask(task3, 2);
    taskListManager.addTask(task4, 2);

    ASSERT_EQ(taskListManager.getNumberOfNotCompletedTask(), 1);
    taskListManager.addTask(task1, 1);
    ASSERT_EQ(taskListManager.getNumberOfNotCompletedTask(), 2);
    taskListManager.addTask(task1, 1);
    taskListManager.addTask(task1, 2);
    ASSERT_EQ(taskListManager.getNumberOfNotCompletedTask(), 4);
    taskListManager.addTask(task2, 1);
    ASSERT_EQ(taskListManager.getNumberOfNotCompletedTask(), 4);
}


//TEST(TestTaskListManager, TaskListManager) {}