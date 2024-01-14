#include "gtest/gtest.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <array>

#include "treeLib.h"

#define BIG_SIZE 1000

/*
 * tree_create
 */

TEST(TreeCreateTest, Empty) {
  struct tree t;

  tree_create(&t);

  EXPECT_TRUE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 0u);
  EXPECT_EQ(tree_height(&t), 0u);

  tree_destroy(&t);
}


/*
 * tree_insert
 */

TEST(TreeInsertTest, OneElement) {
  struct tree t;

  tree_create(&t);
  bool inserted = tree_insert(&t, 0);

  EXPECT_TRUE(inserted);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 1u);
  EXPECT_EQ(tree_height(&t), 1u);
  EXPECT_TRUE(tree_contains(&t, 0));

  tree_destroy(&t);
}

TEST(TreeInsertTest, TwoElements) {
  struct tree t;

  tree_create(&t);
  bool inserted1 = tree_insert(&t, 1);
  bool inserted2 = tree_insert(&t, 2);

  EXPECT_TRUE(inserted1);
  EXPECT_TRUE(inserted2);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 2u);
  EXPECT_EQ(tree_height(&t), 2u);
  EXPECT_TRUE(tree_contains(&t, 1));
  EXPECT_TRUE(tree_contains(&t, 2));

  tree_destroy(&t);
}

TEST(TreeInsertTest, AlreadyPresent) {
  struct tree t;
  tree_create(&t);

  bool inserted1 = tree_insert(&t, 5);

  EXPECT_TRUE(inserted1);

  bool inserted2 = tree_insert(&t, 7);

  EXPECT_TRUE(inserted2);

  bool inserted3 = tree_insert(&t, 7); // already present

  EXPECT_FALSE(inserted3);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 2u);
  EXPECT_EQ(tree_height(&t), 2u);
  EXPECT_TRUE(tree_contains(&t, 5));
  EXPECT_TRUE(tree_contains(&t, 7));

  tree_destroy(&t);
}


TEST(TreeInsertTest, ManyElements) {
  static const int values[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(values); ++i) {
    bool inserted = tree_insert(&t, values[i]);
    EXPECT_TRUE(inserted);
  }

  for (std::size_t i = 0; i < std::size(values); ++i) {
    EXPECT_TRUE(tree_contains(&t, values[i]));
  }

  for (int i = 1; i <= 19; i += 2) {
    EXPECT_FALSE(tree_contains(&t, i));
  }

  tree_destroy(&t);
}

static size_t log_2(size_t x) {
  size_t res = 0;

  while (x != 0) {
    res++;
    x /= 2;
  }

  return res;
}

TEST(TreeInsertTest, Stressed) {
  struct tree t;
  tree_create(&t);

  std::size_t expected = 0;
  std::srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = std::rand();

    if (tree_insert(&t, value)) {
      ++expected;
    }

    EXPECT_EQ(tree_size(&t), expected);
  }

  EXPECT_FALSE(tree_empty(&t));

  size_t size = tree_size(&t);
  size_t height = tree_height(&t);

  EXPECT_EQ(size, expected);
  EXPECT_LE(height, size);
  EXPECT_LE(log_2(size + 1), height);

  tree_destroy(&t);
}

/*
 * tree_remove
 */

TEST(TreeRemoveTest, ManyElements) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_TRUE(tree_contains(&t, origin[i]));

    bool removed = tree_remove(&t, origin[i]);

    EXPECT_TRUE(removed);
    EXPECT_FALSE(tree_contains(&t, origin[i]));
    EXPECT_EQ(tree_size(&t), std::size(origin) - i - 1);
  }

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentBefore) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 1);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentMiddle) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 9);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentAfter) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 19);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, Stressed) {
  struct tree t;
  tree_create(&t);

  std::srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    tree_insert(&t, value);
  }

  size_t expected = tree_size(&t);
  EXPECT_TRUE(expected > 0);

  std::srand(0); // reinit

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();

    bool present = tree_contains(&t, value);

    if (present) {
      --expected;
    }

    bool removed = tree_remove(&t, value);

    EXPECT_EQ(present, removed);
    EXPECT_EQ(tree_size(&t), expected);
  }

  EXPECT_EQ(tree_size(&t), 0u);

  tree_destroy(&t);
}

/*
 * tree_walk_in_order
 */

static void check_tree(int value, void *user_data) {
  int *expected = static_cast<int *>(user_data);

  ASSERT_TRUE(expected != NULL);
  EXPECT_EQ(*expected, value);

  (*expected) += 2;
}

TEST(TreeWalkInOrderTest, Ordered) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  EXPECT_EQ(tree_size(&t), std::size(origin));

  int expected = 2;
  tree_walk_in_order(&t, check_tree, &expected);
  EXPECT_EQ(expected, 20);

  tree_destroy(&t);
}

static void check_once(int value, void *user_data) {
  int *count = static_cast<int *>(user_data);

  ASSERT_TRUE(count != NULL);
  ASSERT_TRUE(2 <= value && value <= 18);
  EXPECT_EQ(count[value / 2], 0);

  count[value / 2] = 1;
}

TEST(TreeWalkInOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_in_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

/*
 * tree_walk_pre_order
 */

TEST(TreeWalkPreOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_pre_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

/*
 * tree_walk_post_order
 */

TEST(TreeWalkPostOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_post_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
