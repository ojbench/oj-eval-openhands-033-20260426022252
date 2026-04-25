// Provided by Lhtie 2022.2

#include "class-bint.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "list.hpp"

#include <list>

const int N = 5e4;

template <typename T>
bool equal(const std::list<T> &x, const sjtu::list<T> &y) {
  if (x.empty() && y.empty()) {
    return true;
  }
  return x.front() == y.front() && x.back() == y.back() && x.size() == y.size();
}

bool testConstructors() {
  std::list<int> ans;
  sjtu::list<int> myList;

  if (ans.size() != myList.size())
    return false;
  for (int i = 0; i < N; ++i) {
    ans.push_back(i);
    myList.push_back(i);
  }

  sjtu::list<int> *otherList = new sjtu::list<int>(myList);
  if (!equal(ans, *otherList)) {
    delete otherList;
    return false;
  }
  delete otherList;
  if (!equal(ans, myList))
    return false;

  return true;
}

bool testAssignment() {
  std::list<int> ans;
  sjtu::list<int> myList;
  for (int i = 0; i < N; ++i) {
    ans.push_back(N - i);
    myList.push_back(N - i);
  }

  myList = myList;
  if (!equal(ans, myList))
    return false;

  sjtu::list<int> otherList;
  otherList = myList;
  if (!equal(ans, otherList))
    return false;

  return true;
}

bool testPush() {
  std::list<int> ans;
  sjtu::list<int> myList;

  for (int i = 0; i < N; ++i) {
    if (rand() % 2) {
      ans.push_back(i);
      myList.push_back(i);
    } else {
      ans.push_front(i);
      myList.push_front(i);
    }
  }

  return equal(ans, myList);
}

bool testPop() {
  std::list<int> ans;
  sjtu::list<int> myList;

  for (int i = 0; i < N; ++i) {
    ans.push_back(i);
    myList.push_back(i);
  }

  for (int i = 0; i < rand() % N; ++i) {
    if (rand() % 2) {
      ans.pop_front();
      myList.pop_front();
    } else {
      ans.pop_back();
      myList.pop_back();
    }
  }

  return equal(ans, myList);
}

bool testBint() {
  std::list<Util::Bint> ans;
  sjtu::list<Util::Bint> myList;
  Util::Bint large = Util::Bint(rand());

  for (int i = 0; i < N / 30; ++i) {
    ans.push_back(Util::Bint(i) * large);
    myList.push_back(Util::Bint(i) * large);
  }

  return equal(ans, myList);
}

bool testInteger() {
  std::list<Integer> ans;
  sjtu::list<Integer> myList;

  for (int i = 0; i < N; ++i) {
    ans.push_back(Integer(N - i));
    myList.push_back(Integer(N - i));
  }

  return equal(ans, myList);
}

bool testMatrix() {
  using Matrix = Diamond::Matrix<double>;
  std::list<Matrix> ans;
  sjtu::list<Matrix> myList;

  for (int i = 0; i < N / 30; ++i) {
    ans.push_back(Matrix(2, 3, i) * Matrix(3, 4, i));
    myList.push_back(Matrix(2, 3, i) * Matrix(3, 4, i));
  }

  return equal(ans, myList);
}

bool testElementAccess() {
  std::list<int> ans;
  sjtu::list<int> myList;
  for (int i = 0; i < N; ++i) {
    ans.push_back(i);
    myList.push_back(i);
    if (ans.front() != myList.front() || ans.back() != myList.back())
      return false;
  }
  return true;
}

bool testCapacityInfo() {
  std::list<int> ans;
  sjtu::list<int> myList;
  for (int i = 0; i < rand() % N; ++i) {
    ans.push_back(i);
    myList.push_back(i);
  }
  if (ans.empty() != myList.empty() || ans.size() != myList.size())
    return false;

  myList.clear();
  return myList.empty() && myList.size() == 0;
}

int main() {
  srand(time(NULL));
  bool (*testList[])() = {
      testConstructors, testAssignment,    testPush,
      testPop,          testBint,          testInteger,
      testMatrix,       testElementAccess, testCapacityInfo};
  const char *Messages[] = {
      "Test 1: Testing default & copy constructors and destructor...",
      "Test 2: Testing operator= ...",
      "Test 3: Testing push_front & push_back...",
      "Test 4: Testing pop_front & pop_back...",
      "Test 5: Testing class-bint...",
      "Test 6: Testing class-integer...",
      "Test 7: Testing class-Matrix...",
      "Test 8: Testing front() & back()...",
      "Test 9: Testing information of capacity..."};

  bool okay = true;
  for (int i = 0; i < sizeof(testList) / sizeof(testList[0]); ++i) {
    printf("%s", Messages[i]);
    if (testList[i]()) {
      printf("Passed\n");
    } else {
      okay = false;
      printf("Failed\n");
    }
  }

  if (okay)
    printf("Congratulations, you have passed all tests!\n");
  else
    printf("Unfortunately, you failed in some of the tests.\n");
  return 0;
}