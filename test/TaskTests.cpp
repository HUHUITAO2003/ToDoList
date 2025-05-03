#include <gtest/gtest.h>
#include "../Task.h"

TEST(TestTask, TaskToString) {
    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(2, "titolo2", "descrizione2", 1, true);

    string string1 = "TaskID: 1 | [ ] - titolo1 - \ndescrizione1";
    string string2 = "TaskID: 2 | [x] - titolo2 - \ndescrizione2";
    EXPECT_EQ(task1.toString(), string1);
    EXPECT_EQ(task2.toString(), string2);
}

TEST(TestTask, TaskSerialize) {
    Task task1(1, "titolo1", "descrizione1", 1);
    Task task2(2, "titolo2", "descrizione2", 1, true);
    EXPECT_EQ("1|1|0|titolo1|descrizione1|", task1.serialize());
    EXPECT_NE("1|1|0|titolo1|descrizione1|", task2.serialize());
}

TEST(TestTask, TaskDeserialize) {
    Task task = Task::deserialize("1|1|0|titolo|descrizione|");
    ASSERT_TRUE(task.getId() == 1);
    ASSERT_TRUE(task.getUrgencyLevel() == 1);
    ASSERT_TRUE(task.isCompleted() == false);
    ASSERT_TRUE(task.getTitle() == "titolo");
    ASSERT_TRUE(task.getDescription() == "descrizione");

    task = Task::deserialize("1|0|1|titolo|descrizione|");
    ASSERT_TRUE(task.getId() == 1);
    ASSERT_TRUE(task.getUrgencyLevel() == 0);
    ASSERT_TRUE(task.isCompleted() == true);
    ASSERT_TRUE(task.getTitle() == "titolo");
    ASSERT_TRUE(task.getDescription() == "descrizione");
}

TEST(TestTask, TaskConstructor) {
    //test eccezioni nel costruttore
    EXPECT_THROW(Task task(-1, "titolo", "descrizione", 0), invalid_id_argument);
    EXPECT_THROW(Task task(-1, "titolo|", "descrizione|", -5), invalid_id_argument);
    EXPECT_THROW(Task task(1, "titolo|", "descrizione", 5), invalid_title_argument);
    EXPECT_THROW(Task task(1, "titolo", "descrizione|", 5), invalid_description_argument);
    EXPECT_THROW(Task task(1, "titolo", "descrizione", 5), invalid_urgencyLevel_argument);
    EXPECT_NO_THROW(Task task(1, "titolo", "descrizione", 0));

    //test costruzione corretta dell'oggetto
    Task task(1, "titolo", "descrizione", 0);
    ASSERT_TRUE(task.getId() == 1);
    ASSERT_TRUE(task.getUrgencyLevel() == 0);
    ASSERT_TRUE(task.isCompleted() == false);
    ASSERT_TRUE(task.getTitle() == "titolo");
    ASSERT_TRUE(task.getDescription() == "descrizione");
}

TEST(TestTask, TaskContains) {
    Task task1(1, "Titolo_Task1", "Descrizione_Task1", 2);
    Task task2(2, "Titolo_Task2", "Descrizione_Task2", 1, true);
    ASSERT_TRUE(task1.contains("Titolo"));
    ASSERT_TRUE(task1.contains("Task"));
    ASSERT_TRUE(task1.contains("_"));
    ASSERT_TRUE(task1.contains("1"));
    ASSERT_TRUE(task1.contains("Descrizione"));
    ASSERT_TRUE(task1.contains("Alto"));
    ASSERT_FALSE(task1.contains("2"));
    ASSERT_FALSE(task1.contains("Basso"));
    ASSERT_FALSE(task1.contains("Medio"));
    ASSERT_FALSE(task1.contains("Critico"));
    ASSERT_TRUE(task2.contains("Titolo"));
    ASSERT_TRUE(task2.contains("Task"));
    ASSERT_TRUE(task2.contains("_"));
    ASSERT_TRUE(task2.contains("2"));
    ASSERT_TRUE(task2.contains("Descrizione"));
    ASSERT_TRUE(task2.contains("Medio"));
    ASSERT_FALSE(task2.contains("1"));
    ASSERT_FALSE(task2.contains("Basso"));
    ASSERT_FALSE(task2.contains("Critico"));
    ASSERT_FALSE(task2.contains("Alto"));
}