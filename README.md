# PHILOSOPHERS

---

***This project is a creater whit part of the curriculum the 42 for aialonso***

## Description

---

This project is an implemantation of the classiscal Dining Philosophers Promblem
using threads and mutexes in C.

The goal is to simulate philosophers sitining around a table alternating between
eating sleeping and thinking while sharing limited resorces (forks) withut
causing deadlocks or starvation.

Each philosopher needs:
- Two forks to eat
- To eat before `time_to_die` expires
- To avoid deadlocks
- To avoid starvation

The simulation must:
- Stop when a philosopher dies.
- Stop when all philosophers have eaten must_eat times (if specified).

## Instructions

---

First, you need to compile the project using:

Once the compilation is complete, you can run
the program from the terminal with:

```
./philo <arguments>
```

The program requires **at least four arguments** and **at most five**.

If fewer than four arguments are provided, the program will not start.
If more than five arguments are provided, the program will exit with an error.

You are free to experiment with different values to observe how the simulation
behaves.

## Resources

---

For this project, I mainly relied on AI assistance to better understand:

- Common concurrency issues such as deadlocks and race conditions.
- The correct usage of thread-related functions (`pthread_create`, `pthread_join`).
- The behavior and proper use of mutexes.

AI was also helpful in debugging complex synchronization issues
that are not always easy to detect at first glance.