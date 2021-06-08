// Banker's Algorithm
// https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/
#include <iostream>
#include <vector>
using namespace std;

// 银行家算法，本算法的目的是找到正确的运行进程的顺序

int main() {
	// P0, P1, P2, P3, P4 are the Process names here

	const int n = 5; // Number of processes
  const int m = 3; // Number of resources types
  int i, j, k;

  /* Allocation Matrix(n * m)
  Allocation[i, j] = k means process Pi is currently allocated ‘k’ instances of resource type Rj
  */
  vector<vector<int>> alloc =  { { 0, 1, 0 },   // P0 
		                     				 { 2, 0, 0 },   // P1
		                     				 { 3, 0, 2 },   // P2
		                     				 { 2, 1, 1 },   // P3
		                     				 { 0, 0, 2 } }; // P4

  /* MAX Matrix(n * m)
  Max[i, j] = k means process Pi may request at most ‘k’ instances of resource type Rj.
  */
	vector<vector<int>> max = { { 7, 5, 3 },   // P0 
				                    	{ 3, 2, 2 },   // P1
				                    	{ 9, 0, 2 },   // P2
				                    	{ 2, 2, 2 },   // P3
				                    	{ 4, 3, 3 } }; // P4
  /* Available Resources
  avail[j] = k means there are ‘k’ instances of resource type Rj
  */
	vector<int> avail = { 3, 3, 2 }; 

  // finish[i] == true表示进程i运行完毕，其资源将被释放
  vector<bool> finish(n);

  // ans[i]是进程号。
  // ans[0] = 2，表示第一个运行的是进程2；ans[3] = 1，表示第4个运行的是进程1
  vector<int> ans(n);

  /* 2-d array of size ‘n*m’ that indicates the remaining resource need of each process.
  Need [i, j] = k means process Pi currently need ‘k’ instances of resource type Rj
    for its execution.
  Need [i, j] = Max[i, j] – Allocation[i, j]
  */
  vector<vector<int>> need(n, vector<int>(m));
  /* content of need will be { { 7, 4, 3 },   
				                     	 { 1, 2, 2 },   
				                     	 { 6, 0, 0 },   
				                     	 { 0, 1, 1 },   
				                       { 4, 3, 3 } }; 
  */
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			need[i][j] = max[i][j] - alloc[i][j];
    }
	}
  // 表示进程运行的顺序
  int ind = 0;
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < n; i++) {
			if (finish[i] == false) {
        // 默认进程i可以运行
				bool can_run = true;
				for (j = 0; j < m; j++) {
					if (need[i][j] > avail[j]) {
            // 进程i需要的资源j多于空闲的资源j
            // 进程i不能运行
						can_run = false;
						break;
					}
				}

				if (can_run == true) {
					ans[ind++] = i;
					for (y = 0; y < m; y++) {
            // 运行进程i后，释放该进程占用的资源
						avail[y] += alloc[i][y];
          }
					finish[i] = true;
				}
			}
		}
	}

	cout << "Following is the SAFE Sequence" << endl;
	for (i = 0; i < n - 1; i++) {
		cout << " P" << ans[i] << " ->";
  }
	cout << " P" << ans[n - 1] <<endl;
/*Following is the SAFE Sequence
 P1 -> P3 -> P4 -> P0 -> P2
*/
	return (0);
}

// This code is contributed by SHUBHAMSINGH10
