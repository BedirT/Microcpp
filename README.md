# Spoc++
      Spelled Out with C++ 

Hello there! This is my rendition of the [Zero to Hero series with Andrej Karpathy](https://youtube.com/playlist?list=PLAqhIrjkxbuWI23v9cThsA9GvCAUhRvKZ) with additional tools and all written in C++. I had a lot of fun working on this project, and I recommend anyone who is remotely interested in any of the topics to give it a whirl, this might be the best series I have followed ever.

Here I will try to summarize and document my implementations, mostly mentioning the general guidelines and the differences from his implementations. I am not an expert in C++, especially in ML topics, but I think the implementation doesn't have much imperfections; if you do have any implementation improvement suggestions please do contribute and let me know why you think the change made makes an improvement over the existing code.

## Table of Contents
### Micrograd
- Custom value and gradient system
- Graphviz visualization of the computational graph
- Fully working neural network implementation

Micrograd is the minimalistic neural network system implementation from scratch. It is a very good introduction to the concepts of neural networks and how they work. It is also a good introduction to the concepts of automatic differentiation and computational graphs. The implementation is very minimalistic and is a good starting point for anyone who wants to learn about neural networks and how they work. All from video 1 of the series, do check it out!

#### Couple notes on the implementation
Using the graph system needs a root node:

```cpp
auto x = var(3.0);
auto y = var(4.0);
auto z = x * y;
auto gs = graph_system(z);
gs.draw();
```

![Graphviz visualization of the computational graph]()

The value class have basic operations overloaded:

```cpp
auto x = var(3.0);
auto y = var(4.0);
auto z = x * y;
z.value(); // 12
auto z = x + y;
z.value(); // 7
auto z = x - y;
z.value(); // -1
auto z = x / y;
z.value(); // 0.75
// or more complex operations
auto z = (x + y) * (x + y);
z.value(); // 49
```