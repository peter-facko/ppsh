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
$ git clone https://github.com/papundekel/PPshell
$ cd PPshell
$ ./build.sh ./ <build-path>
$ cmake --install <build-path> --config Release --prefix <your-installation-path>
$ <your-installation-path>/PPshell
```

#### Runtime Dependencies

- `Boost` >=1.74
- `readline` >=8.2

## Libraries Used

- [`Boost Spirit X3`](https://www.boost.org/doc/libs/release/libs/spirit/doc/x3/html/index.html) - script parsing
- [`Boost Program Options`](https://www.boost.org/doc/libs/release/doc/html/program_options.html) - CLI parsing
- [`readline`](https://tiswww.case.edu/php/chet/readline/rltop.html) - interactive mode

### Development

- [`CMake`](https://cmake.org/) - build system
- [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) - code formatting
- [`pre-commit`](https://pre-commit.com/) - git commit hooks

### Testing

- [`pytest`](https://docs.pytest.org/en/6.2.x/) - integration tests
- [`pytest-asyncio-cooperative`](https://github.com/willemt/pytest-asyncio-cooperative) - tests run asynchronously

## Developing

### Development Dependencies

- `Python` >=3.8
- `pre-commit` >=3.5
- `CMake` >=3.21
- `GCC` >=13
- \+ all runtime dependencies

```bash
$ python -m venv .venv/
$ source .venv/bin/activate
$ pip install -r requirements-dev.txt
$ pre-commit install
```

### Testing

```bash
$ pytest tests/tests --verbose --showlocals
```

#### Additional Dependencies

- `pytest` >=7.4
- `pytest-asyncio-cooperative` >=0.31

```bash
$ pip install -r tests/requirements.txt
```
