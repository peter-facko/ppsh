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

## Libraries Used

- [`Boost Spirit X3`](https://www.boost.org/doc/libs/release/libs/spirit/doc/x3/html/index.html) - parsing
- [`Boost Program Options`](https://www.boost.org/doc/libs/release/doc/html/program_options.html) - CLI
- [`readline`](https://tiswww.case.edu/php/chet/readline/rltop.html) - interactive mode
