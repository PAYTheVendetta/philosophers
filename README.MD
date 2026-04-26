# philosophers

Implementation of the classic **Dining Philosophers Problem** using threads and mutexes in C — part of the 42 Madrid curriculum.

The goal: simulate philosophers sitting around a table, alternating between eating, sleeping and thinking, while sharing limited resources (forks) — without deadlocks or starvation.

---

## Project Structure

```
philosophers/
├── src/
│   ├── main.c              # Entry point and thread joining
│   ├── init.c              # Initialization of rules and philosophers
│   ├── init_threads.c      # Thread creation
│   ├── routine.c           # Philosopher lifecycle (eat/sleep/think)
│   ├── actions.c           # Fork handling and state printing
│   ├── actions_waiter.c    # Waiter mutex logic
│   ├── monitor.c           # Death and meal count monitoring
│   ├── parse.c             # Argument parsing and validation
│   ├── time.c              # Time utilities
│   └── cleanup.c           # Memory freeing and cleanup
├── inc/
│   └── philo.h             # Structs and function prototypes
├── makefile
└── README.md
```

---

## The Problem

N philosophers sit around a circular table. Between each pair of philosophers there is one fork — N philosophers, N forks.

To eat, a philosopher needs **both** the left and right fork. After eating, they put the forks down and sleep, then think, then try to eat again. If a philosopher goes too long without eating, they die.

The challenge: prevent **deadlocks** (everyone waiting forever) and **starvation** (a philosopher never getting to eat).

---

## Solution

This implementation uses a **waiter** pattern with a dedicated mutex to coordinate fork access:

- Each philosopher requests permission from the waiter before picking up forks
- The waiter prevents scenarios where all philosophers grab one fork simultaneously
- A monitor thread continuously checks for deaths and meal completion
- All shared state is protected with mutexes to prevent race conditions

---

## Arguments

```bash
./philo <nb_philos> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat]
```

| Argument | Description |
|---|---|
| `nb_philos` | Number of philosophers (and forks) |
| `time_to_die` | Time in ms before a philosopher dies without eating |
| `time_to_eat` | Time in ms a philosopher takes to eat |
| `time_to_sleep` | Time in ms a philosopher sleeps |
| `must_eat` | *(Optional)* Simulation stops when all philosophers have eaten this many times |

---

## Usage

### Compilation

```bash
make        # compile
make clean  # remove object files
make fclean # remove object files and binary
make re     # full rebuild
```

### Running

```bash
# 5 philosophers, die in 800ms, eat in 200ms, sleep in 200ms
./philo 5 800 200 200

# Same but stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: single philosopher (can never eat — should die)
./philo 1 800 200 200
```

---

## What I Learned

- Threads — `pthread_create`, `pthread_join`
- Mutexes — `pthread_mutex_lock`, `pthread_mutex_unlock`
- Race conditions and how to prevent them
- Deadlock detection and avoidance strategies
- Precise time management with `gettimeofday`
- The waiter pattern as a synchronization solution

---

## Author

**Aitor Alonso Jusdado** — [42 Madrid](https://www.42madrid.com/) — [LinkedIn](https://www.linkedin.com/in/aitor-alonso-jusdado-0ba015153)