# Range Tracker

Library to keep track of half open intervals, providing get, add, and delete
APIs.

I've included a set based method and a segment tree based method (plus a
trivial approach for ground truth). Docs in the corresponding files.


## Usage
Execute make to build the binary.
```
make
```
Afterwards the help command will show the usage instructions:
```
./range-tracker help

usage:
./range-tracker cmdline (segtree|set|trivial)
./range-tracker perf
```
Use `./range-tracker cmdline segtree` for example to play around with the
segtree implementation from the command line. Commands are of the form
```
add 0 30
del 10 20
get 0 30
>> [(0, 10) (20, 30)]
```

Use `./range-tracker perf` to run a simple perf test, comparing the seg tree
and the set based solutions. Here's a sample:
```
number of queries: 1000000
|a|,|b| <=       1000,    workload type:   1/3 uniform:     set based : 0.22196s     seg tree : 2.87515s
|a|,|b| <=       1000,    workload type:       90% get:     set based : 0.09177s     seg tree : 3.84971s
|a|,|b| <=       1000,    workload type:   90% add/del:     set based : 0.26355s     seg tree : 1.86271s
|a|,|b| <=     100000,    workload type:   1/3 uniform:     set based : 0.25330s     seg tree : 3.78593s
|a|,|b| <=     100000,    workload type:       90% get:     set based : 0.11680s     seg tree : 5.28449s
|a|,|b| <=     100000,    workload type:   90% add/del:     set based : 0.26986s     seg tree : 2.89687s
|a|,|b| <= 1000000000,    workload type:   1/3 uniform:     set based : 0.21640s     seg tree : 6.54431s
|a|,|b| <= 1000000000,    workload type:       90% get:     set based : 0.08222s     seg tree : 8.07530s
|a|,|b| <= 1000000000,    workload type:   90% add/del:     set based : 0.28421s     seg tree : 5.19850s
```

(On a side note, I expected the seg tree approach to work faster in smaller
ranges, which is why I implemented it in the first place. Unfortunately this
didn't turn out to be correct. I played around with the perf tool a little
bit to see what's causing the problem, but haven't found much.)

To build and run the tests:
```bash
make run-tests
```
Hang tight.. it will take a while.


To build doxygen docs:
```bash
make doc
```

## License
MIT
