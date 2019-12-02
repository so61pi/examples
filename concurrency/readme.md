The goal here is to find out if we can run multiple threads simultaneously to achieve maximum concurrency but end-users still see the operations as if they run serially.

A simple assumption is end-users cannot actually see/guess the actual execution sequence of operations, like by looking at the logs.

### Read-Check-Update

We have 4 threads carrying out actions based on shared data `data-1` and `data-2`. The shared data can be in-process variables, or placed externally in a database. Threads can be actual threads, or multiple instances of an application.

- Thread X

  1. Read `data-1` into `ld`
  2. Check `ld`
  3. Update `data-2` based on the verdict

- Thread Y

  1. Disable `data-1`

- Thread Z

  1. Update `data-2`

- Thread W

  1. Disable `data-1`
  2. Update `data-2`

Now we will analyze the interaction between X and Y. **Y.1** only has effect on **X.1** => **Y.1** is independent from **X.2** and **X.3** => execution order between **Y.1**, **X.2**, **X.3** doesn't affect end result => X and Y can run concurrently as it has same effects as running serially (`X.1 -> Y.1 -> X.2 -> X.3` = `X.1 -> X.2 -> X.3 -> Y.1`).

Similar for X and Z.

However, it's different for X and W because

- **W.1** and **X.1** are dependent.
- **W.2** and **X.3** are dependent.

In case **W.1** happens before **X.1** then the result must be like W happens before X, so **W.2** must happens before **X.3** in all possible execution paths. This is achievable in concurrent setting because by letting **W.1** run first, thread X will never get to **X.3**.

In contrast, if **W.1** happens after **X.1** then the result must be like W happens after X, so **W.2** must happens after **X.3** in all possible execution paths. We cannot guarantee this if X and W run concurrently because the order of **W.2** and **X.3** is uncontrollable.

Consequently, X and W must actually be run serially.

### Read-Modify-Write

In this scenario, two threads X and Y share the same code

1. Read `data` into `ld`
2. Change `ld`
3. Save `ld` back to `data`

The dependencies between operations are:

- **X.3** and **Y.3** are dependent.
- If base on the operation at 2 we can infer the last value of `data` (most likely we can, like `ld = ld + 1`), then

  + **X.1** and **Y.3** are dependent
  + **Y.1** and **X.3** are dependent

So when **X.3** happens before **Y.3**, the observed result must be like if X is before Y. That means we should be able to move **X.3** before **Y.1** freely, which we are not allowed to do as **X.3** and **Y.1** are dependent.

Therefore, X and Y must run serially.
