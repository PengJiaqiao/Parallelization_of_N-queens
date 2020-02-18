# Parallelization_of_N-queens
## Version 0
回溯法，使用三个布尔数组记录每列和每条对角线是否已被占用，然后从上往下依次试探每行的皇后可以放在哪些列。
```
n = 16, result = 14772512, Total time:657.832

n = 17, result = 95815104, Total time:4788.52
```

## Version 1
version 0的深度优化版，用整形变量的每一位代替布尔数组的每一个布尔变量，所有步骤全部用位运算代替。
```
n = 16, result = 14772512, Total time:15.7722

n = 17, result = 95815104, Total time:111.375

n = 18, result = 666090624, Total time:815.989

n = 19, result = 4968057848, Total time:6228.92
```

## Version 2
version 1的并行版本，设计了一个taskPool类管理任务栈，设计了一个自旋锁类，子线程自行从栈中取任务计算。子线程可以在栈中任务数量不足的时候，将任务分解为当前行可行位置放置皇后之后从下一行开始的数个子任务并压入栈，实现简单而可靠的负载均衡。  
8核CPU开8线程约为单线程速度的5.5倍。
```
n = 16, result = 14772512, Total time:3.06581

n = 17, result = 95815104, Total time:21.2106

n = 18, result = 666090624, Total time:154.903

n = 19, result = 4968057848, Total time:1222.11

n = 20, result = 39029188884, Total time:9370.54
```