# PPshell

A practice implementation of a Unix shell in modern C++.

## Highlighted Features

- different sources
  - CLI - `PPshell --command "cmd"`
  - script - `PPshell script.sh`
  - interactive - `PPshell`
- pipes - `cmd1 | cmd2`
- redirections - `cmd1 <x >y; cmd2 >>z`
- builtins - `cd` and `exit`

## Usage

### Container

```bash
$ [docker|podman] run --interactive --tty docker.io/fackop/pppshell
```

### Local

```bash
$ git clone https://github.com/papundekel/PPshell && cd PPshell
$ ./build.sh ./ <build-path> && cmake --install <build-path> --config Release --prefix <your-installation-path>
$ <your-installation-path>/PPshell
```

## Libraries Used

- [`Boost Spirit X3`](https://www.boost.org/doc/libs/release/libs/spirit/doc/x3/html/index.html) - script parsing
- [`Boost Program Options`](https://www.boost.org/doc/libs/release/doc/html/program_options.html) - CLI parsing
- [`readline`](https://tiswww.case.edu/php/chet/readline/rltop.html) - interactive mode

### Development

- [`CMake`](https://cmake.org/) - build system
- [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) - code formatting
- [`pre-commit`](https://pre-commit.com/) - git commit hooks

## Developing

### Prerequisites

- `Python` >=3.8
- `pre-commit` >=3.5
- `CMake` >=3.21
- `GCC` >=13
- `Boost` >=1.74
- `readline` >=8.2

### Setup

```bash
$ python -m venv .venv/
$ source .venv/bin/activate
$ pip install -r requirements-dev.txt
$ pre-commit install
```
