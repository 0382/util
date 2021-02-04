# argparse

Parse command-line arguments.

## Usage

```c++
// initialize the program with a description
auto args = argparse::argparser("A quantum calculation program");

// you can set program name, if not the `argv[0]` is used as default
args.set_program_name("test")
// support train calls
// for `add_option`
// 1st arg means short option name
// 2nd arg means long option name
// 3rd arg means help message
// 4th arg means default value,
// if not set this option don't need a value
// you should use `get_option<bool>` to get if it is set or not
    .add_option("-o", "--openmp", "use openmp or not")
    .add_option("-m", "--mpi", "use mpi or not")
    .add_option("-t", "--threads", "how many threads to use", 4)
// for `add_argument`
// 1st arg means argument name
// 2ed arg means help message
// all arguments should be set
// it will cause error if argument number is more or less than you have set
    .add_argument("input", "initialize file")
    .parse(argc, argv);

// get the values
std::cout << args.get_argument("input") << std::endl;
std::cout << args.get_option<int>("-t") << std::endl;
std::cout << args.get_option<bool>("--mpi") << std::endl;
```

## Reference

- [cmdline](https://github.com/tanakh/cmdline)