/*
https://www.cnblogs.com/deepllz/p/11533860.html

一种简单的死锁检测算法
1.死锁检测

给定一组线程操作锁的流程，判断是否会发生死锁？

例如:有两个线程和两个资源，线程对锁的操作如下:

其中T表示线程id，L表示锁id，S表示操作(1表示获取锁，0表示释放锁)

T L S
1 1 1(线程1获取1号锁)
2 2 1(线程2获取2号锁)
1 2 1(线程1获取2号锁，保持等待)
2 1 1(线程2获取1号锁，导致死锁)

如果最后一次操作换为:2 2 0，就不会死锁.

问题的关键是如何判断死锁的发生，以上面的例子为例：线程2获取1号锁时，发现1号锁被1号线程占用，那么就要等待线程1释放1号锁，
然后再看线程1在等待2号锁，2号锁被2号线程占用，因此1号线程又要等2号线程释放2号锁，这就形成了一个等待环：线程2->线程1->线程2，发生死锁。
所以检测死锁的方法就是判断是否存在这种等待的环路。

对于给定的线程操作锁的序列：vector<vector<int>> tls，判断是否发生死锁要维护3个unordered_map,

unordered_map<int, int> lock2thread:锁->线程，标识当前锁被哪个线程占用

unordered_map<int, int> waitedthread2lock:标识当前线程在等待哪个锁

unordered_map<int, vector<int>> thread2locks：标识线程持有的锁.

*/

#include <vector>
#include <unordered_map>
#include <algorithm>    // std::find
#include <iostream>
using namespace std;

bool DeadLock(vector<vector<int>> &tls) {
  int size = tls.size();
  unordered_map<int, int> lock2thread; // 锁->线程，标识当前锁被哪个线程占用
  unordered_map<int, int> waitedthread2lock;
  unordered_map<int, vector<int>> thread2locks; 
  for(int i = 0; i < size; i++) {
    int tid = tls[i][0];
    int lock = tls[i][1];
    int state = tls[i][2];
    if (state == 0) { //释放锁，这是一定不会引起死锁的，因此只需要更新3个map就可以了
      //1.从lock2thread中删除key==lock的元素
      lock2thread.erase(lock2thread.find(lock));
      //2.从thread2locks中移除lock
      thread2locks[tid].erase(find(thread2locks[tid].begin(), thread2locks[tid].end(), lock));
      //3.遍历waitedthread2lock，查看哪个线程等待lock
      for (auto it = waitedthread2lock.begin(); it != waitedthread2lock.end(); ++it) {
        //3.1如果有线程等待此lock，那么依次更新lock2thread和thread2locks
        if (it->second == lock) {
          lock2thread[lock] = it->first;
          thread2locks[it->first].push_back(lock);
        }
      }
    } else { // 说明tid想要获取lock，那么这个操作是可能导致死锁的
      if (lock2thread.find(lock) != lock2thread.end()) {
        // 说明该锁已经被占用，那么可能引起死锁
        // 假设现在线程tid想申请锁lock
        // 如果锁lock本身就被线程tid持有，那么就会死锁
        // 如果锁lock被其他线程tid2持有，那么线程tid需要等待。
        // 如果锁lock被线程tid2持有，同时tid2还在等待线程tid持有的锁，也会死锁
        int nexttid = 0;//当前线程依赖的下一个线程
        int nextlock = lock;
        while(true) {
          nexttid = lock2thread[nextlock];
          if (nexttid == tid) {
            return true;//发生死锁
          }
          //查看nexttid在等待哪个资源
          if (waitedthread2lock.find(nexttid) != waitedthread2lock.end()) {
            nextlock = waitedthread2lock[nexttid];
          } else {
            // 说明没有环路，不发生死锁，更新waitedthread2lock;
            waitedthread2lock.insert({tid, nextlock});
            break;
          }
        }
      } else {
        // 说明lock空闲，直接获取，更新lock2thread和thread2locks;
        lock2thread.insert({lock, tid});
        thread2locks[tid].push_back(lock);
      }
    }
  } // end of for loop
  return false;
} // end of Deadlock

int main() {
    bool is_deadlock;
    vector<vector<int>> test_1 = {{1, 1, 1}, {2, 2, 1}, {1, 2, 1}, {2, 1, 1}};
    is_deadlock = DeadLock(test_1);
    cout << (is_deadlock ? "deadlock happend" : "deadlock not happenend") << endl;
    vector<vector<int>> test_2 = {{1, 1, 1}, {2, 2, 1}, {1, 2, 1}, {2, 2, 0}};
    is_deadlock = DeadLock(test_2);
    cout << (is_deadlock ? "deadlock happend" : "deadlock not happenend") << endl;
    return 0;
}