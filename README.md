# AssignmentProblem

Code solutions for [Assignment problem](https://en.wikipedia.org/wiki/Assignment_problem#:~:text=The%20assignment%20problem%20is%20a,on%20the%20agent%2Dtask%20assignment.)
with two approaches:

- naive.cpp contains "brute force" recursive solution which has exponential time complexity
- hungarian.cpp contains [Hungarian algorithm](https://en.wikipedia.org/wiki/Hungarian_algorithm#:~:text=Hungarian%20algorithm,anticipated%20later%20primal%2Ddual%20methods.)
which solve this problem in O(N^3)


You can run this code as example:

```bash
 g++ -o program $SOLUTION_FILE && ./program < input_example.txt
```

