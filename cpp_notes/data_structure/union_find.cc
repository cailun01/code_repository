#include <vector>
using namespace std;
/*
参考：
labuladong公众号：Union-Find 并查集算法详解
https://mp.weixin.qq.com/s?__biz=MzAxODQxMDM0Mw==&mid=2247484751&idx=1&sn=a873c1f51d601bac17f5078c408cc3f6&chksm=9bd7fb47aca07251dd9146e745b4cc5cfdbc527abe93767691732dfba166dfc02fbb7237ddbf&scene=21#wechat_redirect
*/
class UnionFind {
private:
  int count;
  vector<int> parent;
  // 新增一个数组记录树的“重量”，size[3] = 5表示，以节点3为根的那棵树，总共有5个节点。
  vector<int> size;
public:
  UnionFind(int n) {
    this->count = n;
    parent.resize(n);
    size.resize(n);
    // 初始化parent数组
    for (int i = 0; i < n; ++i) {
      parent[i] = i;
      size[i] = i;
    }
  }

  int Find(int x) {
    // 根节点的 parent[x] == x
    while (parent[x] != x) {
      // 进行路径压缩
      parent[x] = parent[parent[x]];
      x = parent[x];
    }
    return x;
  }

  /* 将 p 和 q 连接 */
  void Union(int p, int q) {
    int root_p = Find(p);
    int root_q = Find(q);
    if (root_p == root_q) {
      return;
    }
    // 小树接到大树下面，较平衡
    if (size[root_p] > size[root_q]) {
      parent[root_q] = root_p;
      size[root_p] += size[root_q];
    } else {
      parent[root_p] = root_q;
      size[root_q] += size[root_p];
    }
    count--;
  }
  /* 判断 p 和 q 是否连通 */
  bool IsConnected(int p, int q) {
    int root_p = Find(p);
    int root_q = Find(q);
    return root_p == root_q;
  }
  /* 返回图中有多少个连通分量 */
  int Count() {
    return count;
  }
};