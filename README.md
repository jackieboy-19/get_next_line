*This project has been created as part of the 42 curriculum by jbrito.*

# GET_NEXT_LINE

## Description

`get_next_line` is a C function that reads a text file descriptor one line at a time. Each call returns the next line, including the terminating `\n` (except for the last line if the file does not end with one). It returns `NULL` when there is nothing left to read or when an error occurs.

The project taught me how static variables work in C. `read()` consumes bytes from the file descriptor and cannot give them back, so anything read past the end of the current line must be kept between calls. A static variable holds that leftover text.

The bonus version handles several file descriptors at the same time, keeping the reading state of each one separate, using a single static variable.

Files:

| FILE | Content |
| ---- | ------- |
| `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h` | Mandatory part |
| `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h` | Bonus part (multiple file descriptors) |

## Instructions

Compile with the buffer size of your choice (the default is 1000 if `-D BUFFER_SIZE` is not given):

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c 
```

For the bonus:

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`main.c` is your own test program. It is not part of the project. A minimal one opens a file, calls `get_next_line` in a loop until it returns `NULL`, prints each line and frees it.

## Algorithm

The function keeps a static string (the "stash") with the text that was read but not yet returned. Each call does four steps:

1. **Read** (`ft_read`): while the stash has no `\n` and `read()` still returns bytes, read `BUFFER_SIZE` bytes into a buffer and append it to the stash (`ft_strjoin`)
2. **Extract the line** (`ft_next_line`): copy the stash up to and including the first `\n`, or up to the end of the string if there is none.
3. **Keep the rest** (`ft_get_rest`): build a new stash with what comes after the `\n` and free the old one. If nothing remains, the stash becomes `NULL`.
4. **Return** the extracted line.

The line is extracted before the rest is saved because `ft_get_rest` frees the old stash.

### Why this approach

- **Reading stops at the first `\n`**, so the function reads as little as possible and never loads the whole file, as the subject asks.
- **It works with any `BUFFER_SIZE`**. A small value means more `read()` calls, a large one means fewer calls but a bigger buffer. The result is the same either way.
- **`NULL` means an empty stash**. A `NULL` stash is handled in every helper, so the first call needs no special case, and when nothing remains no empty string has to be allocated.
- **Memory** is freed at each step: `ft_strjoin` frees the old stash, `ft_get_rest` frees the stash it replaces, and `ft_read` frees its buffer, including on error. The caller frees the returned line.

### Trade-off

Every loop iteration searches the stash for `\n` and `ft_strjoin` copies it again, so for a very long line with a very small `BUFFER_SIZE` (for example 1) the cost grows quickly. It is correct, but slow in that extreme case.

### Bonus 

Instead of one stash, the bonus uses an array of stashes indexed by the file descriptor: `static char *str[1024]`. It is still a single static variable. Each file descriptor has its own stash, so reading from fd 3, then 4, then 3 again never mixes their content. File descriptors below 0 or at 1024 and above return `NULL`.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- The 42 subject for get_next_line (version 14.3)
- Valgrind, to check for memory leaks and invalid accesses
- gnlTester by Tripouille, to test the project

### Use of AI

I used Claude (Anthropic) as a tutor throughout the project:

- To understand the problem and design the algorithm, through guiding questions that I answered myself.
- To review the code I wrote step by step and point out bugs (for example the order of conditions with `NULL`, `free` of the old stash, and Norm issues)
- To explain how to test with valgrind and gnlTester and to read their output.
- To write a visual explanation file to prepare for the evaluation, and to help draft this README.
