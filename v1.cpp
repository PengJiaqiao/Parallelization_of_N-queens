#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

long sum;
long upperlim; // 1 <= n <= 32

void solveNQueens(long row, long ld, long rd) // left & right diagonal
{
    if (row != upperlim)
    {
        long pos = upperlim & ~(row | ld | rd);
        while (pos)
        {
            long p = pos & (~pos + 1); // 找到从右起第一个1

            pos -= p; // 去掉最右的空位
            // 逐行求解，对角线形成的禁位在下一行分别左右移1位
            solveNQueens(row + p, (ld | p) << 1, (rd | p) >> 1);
        }
    }
    else
        ++sum;
}

int main()
{
    int n = 1;
    while (true)
    {
        clock_t start = clock();
        sum = 0;
        upperlim = 1;
        upperlim = (upperlim << n) - 1;
        solveNQueens(0, 0, 0);
        clock_t end = clock();
        double time = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "n = " << n << ", result = " << sum << ", Total time:" << time << "\n"
             << endl;
        ++n;
    }
    return 0;
}

/*
n = 1, result = 1, Total time:1e-06

n = 2, result = 0, Total time:0

n = 3, result = 0, Total time:1e-06

n = 4, result = 2, Total time:1e-06

n = 5, result = 10, Total time:1e-06

n = 6, result = 4, Total time:3e-06

n = 7, result = 40, Total time:9e-06

n = 8, result = 92, Total time:4.4e-05

n = 9, result = 352, Total time:0.000163

n = 10, result = 724, Total time:0.000502

n = 11, result = 2680, Total time:0.002318

n = 12, result = 14200, Total time:0.012156

n = 13, result = 73712, Total time:0.064128

n = 14, result = 365596, Total time:0.372695

n = 15, result = 2279184, Total time:2.35846

n = 16, result = 14772512, Total time:15.7722

n = 17, result = 95815104, Total time:111.375

n = 18, result = 666090624, Total time:815.989

n = 19, result = 4968057848, Total time:6228.92
 */