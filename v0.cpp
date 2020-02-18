#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

class Solution
{
public:
    int solveNQueens(int n)
    {
        vector<bool> col(n, false), diagonal_45(2 * n - 1, false), diagonal_135(2 * n - 1, false);
        int result = 0;
        dfs(0, n, col, diagonal_45, diagonal_135, result);
        return result;
    }

private:
    void dfs(int row, const int &n, vector<bool> &col, vector<bool> &diagonal_45,
             vector<bool> &diagonal_135, long &result)
    {
        if (row == n)
        {
            ++result;
            return;
        }

        for (int i = 0; i < n; ++i)
        {
            int idx_45 = row + i;
            int idx_135 = row - i + n - 1;
            if (col[i] == false && diagonal_45[idx_45] == false && diagonal_135[idx_135] == false)
            {
                col[i] = true;
                diagonal_45[idx_45] = true;
                diagonal_135[idx_135] = true;
                dfs(row + 1, n, col, diagonal_45, diagonal_135, result);
                col[i] = false;
                diagonal_45[idx_45] = false;
                diagonal_135[idx_135] = false;
            }
        }
        return;
    }
};

int main()
{
    Solution s;
    int n = 0;
    while (true)
    {
        clock_t start = clock();
        long result = s.solveNQueens(++n);
        clock_t end = clock();
        double time = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "n = " << n << ", result = " << result << ", Total time:" << time << "\n"
             << endl;
    }
    return 0;
}

/* 
n = 1, result = 1, Total time:1.4e-05

n = 2, result = 0, Total time:1e-05

n = 3, result = 0, Total time:1.1e-05

n = 4, result = 2, Total time:2.5e-05

n = 5, result = 10, Total time:6.9e-05

n = 6, result = 4, Total time:0.000225

n = 7, result = 40, Total time:0.000843

n = 8, result = 92, Total time:0.003435

n = 9, result = 352, Total time:0.015376

n = 10, result = 724, Total time:0.03234

n = 11, result = 2680, Total time:0.087569

n = 12, result = 14200, Total time:0.438487

n = 13, result = 73712, Total time:2.59025

n = 14, result = 365596, Total time:15.8626

n = 15, result = 2279184, Total time:100.667

n = 16, result = 14772512, Total time:657.832

n = 17, result = 95815104, Total time:4788.52

n = 18, result = 666090624, Total time:37344
*/