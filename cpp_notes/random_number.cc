/*
快速排序，需要随机取pivot。此做法来自《算法笔记》将快速排序（P144）的部分。
*/

#include <stdlib.h>
#include <time.h>

int main() {
  // srand产生随机种子，会对应一个随机数。如果种子相同，则rand函数产生的随机数相同。
  // 为了防止随机数重复，用系统时间来初始化。
  srand((unsigned)time(NULL));
  int a = 0, b = 10;
  // 在[a, b]之间取一随机数
  // rand函数只能取[0, RAND_MAX]范围内的整数。想要在指定范围[a, b]输出随机数，
  // 需要使用rand() % (b - a + 1) + a。显然rand() % (b - a + 1)的范围是[0, b - a]。再加上a之后就是[a, b]。
  int pivot = rand() % (b - a + 1) + a;
}