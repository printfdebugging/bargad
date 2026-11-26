# Build Instructions

## Environment Setup
- Run [dependencies.sh](./tooling/dependencies.sh)
- On Windows
  - Installs MSYS2, Git, VSCode
  - Installs the compilers and build tools under the `clang64/mingw-w64-clang-x86_64` MSYS2 toolchain
  - Adds `C:\msys64\clang64\bin` to the path
- On MacOS
  - Installs Homebrew
  - Installs the compilers and build tools with `brew`
- On Linux, 
  - Installs the packages

> [!NOTE]
> `dependencies.sh` only installs packages for Archlinux. If you use some
> other distribution, feel free to find the equivalent package names and add
> them to the script, thanks!

## Build Steps
```bash
make init  # Git submodule fetch
make       # Debug build with CMake
code .     # Open in VSCode
```

## IDE Support
### General
- CMake generates a `compile_commands.json`, so any IDE/editor with clangd LSP should work.
- The [.clang-format] and [.editorconfig] files standardize the formatting and indentation rules
- `make reformat` reformats the source files in git, so that can be used from CLI to format the files.
- Neovim & Emacs users can figure this out on their own :wink:
- Qt Creator also works with CMake without much configuration, so that's something to look for if you don't want to use VSCode.

[.clang-format]: ./.clang-format
[.editorconfig]: ./.editorconfig

### VSCode
- [extensions.json] lists some extensions which are nice to have, for debugging, code navigation etc.
- See [.vscode] for an example VSCode config.

[extensions.json]: ./tooling/vscode/.vscode/extensions.json
[.vscode]: ./tooling/vscode/.vscode

# LLM Policy

> [!NOTE]
> It's not about LLMS!

## LLMs are slot machines

I am not in a hurry to finish this project. The problem is not LLMs, it's me.
My brain works in a certain way, and it has a thing for shortcuts. If I leave
some room form LLMs, the thing slowly takes over (becuase the brain prefers
comfort and shortcuts).

## The marathon anology

You don't run a marathon becuase you want to reach the end point as soon as
possible. And if you could, at any time during the marathon, click a button and
get a bike to cover some of the distance with the bike, till you are not tired,
then there's no point in running the marathon. One does not run a marathon to
cover a distance and wear that as a medal. I hope they don't.

## A Badminton match

I don't play a badminton match to win, I don't know if I will win, most of the
times you can't  affect that. What you can do is play one rally at a time and
make sure that your form is correct and that you are not repeating mistakes.
That's how you have fun, and enjoy both wins and losses, because it's not about
wins and losses.

## Failures are necessary

It's good to not succeed in the first go. When you fail, you learn a fact about
the universe which you didn't know before. And LLMs take that out of the
equation. You are just chatting with it and, even if you read the code, you are
not going to absorb the lesson as much as you would do when you hit the wall
yourself. Infact you might not even feel happy about it, as you don't own the
code. You can't call a kid yours if you didn't raise it.

- https://github.com/printfdebugging/engine
- https://github.com/printfdebugging/game
- https://github.com/printfdebugging/externals
- https://github.com/printfdebugging/roots
- https://github.com/printfdebugging/farmhouse

These are some of my previous attempts at this. I am not sure I still get this,
but atleast I am not repeating the mistakes I made there. So keep LLMs away from
my code, fork it and use it for whatever you want, I don't care. Just no LLM
related stuff here. I have tried them and I will never use them, ever again
wherever I have a say in that.