# CSE-633-Parallel-Algorithms
Subset Sum Count using MPI in C

* Code can run on any number of processors.
* It supports multiple columns per processor.
* The number of columns per processors will be dynamically adjusted by the code as per need.

Project Description:

For my semester-long parallel computing project, I implemented a solution to the subset sum count problem using MPI in C. My goal was to determine the number of subsets within an array that sum to a target value. My approach divided the dynamic programming table into column groups, with each processor responsible for computations on one group. Through message passing, the processors shared intermediate results to collectively calculate the final subset count. By testing on large inputs and varying processor counts, I quantified speedup versus sequential execution and studied scaling behavior. This project allowed me to apply course concepts like parallel decomposition, load balancing, and communication overhead. Over the course of the project, I gained hands-on experience with parallel algorithm design, MPI programming, and performance analysis. Completing it satisfied the MS program's project requirement and demonstrated my skills in practical parallel computing.

Reference:
https://www.geeksforgeeks.org/count-of-subsets-with-sum-equal-to-x/
