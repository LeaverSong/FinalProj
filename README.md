# Maximumflow-Dinic-CUDA

## BrainStorm



Since the problem is equivalent to the following description:

First, we define the "water plant" have two states:

- Overload Node:
  $$
  O(u): \sum_{e(v', u)} f(e(u, v)) -  \sum_{e(u, v')} f(e(u, v)) < 0
  $$
  

- Non-Overload Node:
    $$
    N(u): \sum_{e(v', u)} f(e(u, v)) -  \sum_{e(u, v')} f(e(u, v)) \geq 0
    $$

Source Node is always in Non-Overload State, and Target Node is always in Overload State.

the maximum flow problem is equivalent to finding a sum of flow to target under the state that all Nodes are Non-Overload Nodes except the Target node.



**The benefit of this description is that we solve the problem by finding nodes' states instead of considering flow, which will cause BFS or DFS(I think these algorithms are bad for parallelization).**
