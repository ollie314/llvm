//===- unittests/ADT/IListIteratorTest.cpp - ilist_iterator unit tests ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/ilist.h"
#include "gtest/gtest.h"

using namespace llvm;

namespace {

struct Node : ilist_node<Node> {};

TEST(IListIteratorTest, DefaultConstructor) {
  iplist<Node>::iterator I;
  iplist<Node>::reverse_iterator RI;
  iplist<Node>::const_iterator CI;
  iplist<Node>::const_reverse_iterator CRI;
  EXPECT_EQ(nullptr, I.getNodePtr());
  EXPECT_EQ(nullptr, CI.getNodePtr());
  EXPECT_EQ(nullptr, RI.getNodePtr());
  EXPECT_EQ(nullptr, CRI.getNodePtr());
  EXPECT_EQ(I, I);
  EXPECT_EQ(I, CI);
  EXPECT_EQ(CI, I);
  EXPECT_EQ(CI, CI);
  EXPECT_EQ(RI, RI);
  EXPECT_EQ(RI, CRI);
  EXPECT_EQ(CRI, RI);
  EXPECT_EQ(CRI, CRI);
  EXPECT_EQ(I, RI.getReverse());
  EXPECT_EQ(RI, I.getReverse());
}

TEST(IListIteratorTest, Empty) {
  iplist<Node> L;

  // Check iterators of L.
  EXPECT_EQ(L.begin(), L.end());
  EXPECT_EQ(L.rbegin(), L.rend());

  // Reverse of end should be rend (since the sentinel sits on both sides).
  EXPECT_EQ(L.end(), L.rend().getReverse());
  EXPECT_EQ(L.rend(), L.end().getReverse());

  // Iterators shouldn't match default constructors.
  iplist<Node>::iterator I;
  iplist<Node>::reverse_iterator RI;
  EXPECT_NE(I, L.begin());
  EXPECT_NE(I, L.end());
  EXPECT_NE(RI, L.rbegin());
  EXPECT_NE(RI, L.rend());

  // Don't delete nodes.
  L.clearAndLeakNodesUnsafely();
}

TEST(IListIteratorTest, OneNodeList) {
  iplist<Node> L;
  Node A;
  L.insert(L.end(), &A);

  // Check address of reference.
  EXPECT_EQ(&A, &*L.begin());
  EXPECT_EQ(&A, &*L.rbegin());

  // Check that the handle matches.
  EXPECT_EQ(L.rbegin().getNodePtr(), L.begin().getNodePtr());

  // Check iteration.
  EXPECT_EQ(L.end(), ++L.begin());
  EXPECT_EQ(L.begin(), --L.end());
  EXPECT_EQ(L.rend(), ++L.rbegin());
  EXPECT_EQ(L.rbegin(), --L.rend());

  // Check conversions.
  EXPECT_EQ(L.rbegin(), L.begin().getReverse());
  EXPECT_EQ(L.begin(), L.rbegin().getReverse());

  // Don't delete nodes.
  L.clearAndLeakNodesUnsafely();
}

TEST(IListIteratorTest, TwoNodeList) {
  iplist<Node> L;
  Node A, B;
  L.insert(L.end(), &A);
  L.insert(L.end(), &B);

  // Check order.
  EXPECT_EQ(&A, &*L.begin());
  EXPECT_EQ(&B, &*++L.begin());
  EXPECT_EQ(L.end(), ++++L.begin());
  EXPECT_EQ(&B, &*L.rbegin());
  EXPECT_EQ(&A, &*++L.rbegin());
  EXPECT_EQ(L.rend(), ++++L.rbegin());

  // Check conversions.
  EXPECT_EQ(++L.rbegin(), L.begin().getReverse());
  EXPECT_EQ(L.rbegin(), (++L.begin()).getReverse());
  EXPECT_EQ(++L.begin(), L.rbegin().getReverse());
  EXPECT_EQ(L.begin(), (++L.rbegin()).getReverse());

  // Don't delete nodes.
  L.clearAndLeakNodesUnsafely();
}

TEST(IListIteratorTest, CheckEraseForward) {
  iplist<Node> L;
  Node A, B;
  L.insert(L.end(), &A);
  L.insert(L.end(), &B);

  // Erase nodes.
  auto I = L.begin();
  EXPECT_EQ(&A, &*I);
  EXPECT_EQ(&A, L.remove(I++));
  EXPECT_EQ(&B, &*I);
  EXPECT_EQ(&B, L.remove(I++));
  EXPECT_EQ(L.end(), I);

  // Don't delete nodes.
  L.clearAndLeakNodesUnsafely();
}

TEST(IListIteratorTest, CheckEraseReverse) {
  iplist<Node> L;
  Node A, B;
  L.insert(L.end(), &A);
  L.insert(L.end(), &B);

  // Erase nodes.
  auto RI = L.rbegin();
  EXPECT_EQ(&B, &*RI);
  EXPECT_EQ(&B, L.remove(&*RI++));
  EXPECT_EQ(&A, &*RI);
  EXPECT_EQ(&A, L.remove(&*RI++));
  EXPECT_EQ(L.rend(), RI);

  // Don't delete nodes.
  L.clearAndLeakNodesUnsafely();
}

} // end namespace
