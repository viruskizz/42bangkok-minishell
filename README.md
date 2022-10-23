<div align=center >
    <h1>minishell</h1>
    <img src="https://raw.githubusercontent.com/viruskizz/viruskizz-myutils/main/minishell_badge.png" alt="minishell badge" />
</div>

## _42Bangkok' project_



The **minishell** 42 project, recoding our own little bash. A program capable of parsing a prompt and launching executables with arguments, along with a few built-in functions.

**Read documentation:**

- [Shell Command Language by POS] *
- [Write your own shell] *
- [Bash Reference Manual by GNU]

\* is recommended documentation.

## Shell Grammar and Terminoly

There are descrition that you should about shell.

### Field

### Token

### Quoting

### Word Expansion

### List

### Pipeline

### Wildcard

### Simple Command

### Command Table

---

## Implementation

The step to implement your `minishell` that can split into 3 path.

1. Handing Signal and readline
2. Handling Input
3. การ run command จากคำสั่งที่ได้รับ

### PART 1: Handling Signal and `readline`

1.1 Add `readline()` function in `while()` with unfinite loop condition. It will exit loop with `exit` input or signal `Ctrl+D`
1.2 Add `sigaction` to customize general signal to yours. read more in [Signal Number]
    - `Ctrl + C` is `SIGINT`, the interupt signal
    - `Ctrl + \` is `SIGQUIT`, the quit signal or end of input signal.
    - `Ctrl + D` is `EOF`, End of file.

### PART 2: Handling Input

#### 2.1 Simple cleaning

Simple handing and validation input string with `NULL` `newline` `empty` `space` etc.
**Input:**

```sh
# input
$minishell: echo    testing $HOMEx      "path$HOME"        'single' && ls src* | wc -l
```

#### 2.2 Split input

Split string input to token by field (Lexical Analyzer).
The fields are handled.

- Space ` `
- Tab `\t`
- Newline `\n`
- End of string `\0`

No need to split quoting metachar to single token. So important to still keep single and double quoting in string. Because we need to handle them after.

**Result:**

```c
echo, testing, $HOMEx, "path$HOME", 'single', &&, ls, src*, |, wc, -l
```

#### 2.3 Paring token

To parse single to token that including with special parameter. by ordering

- parse environment variable to string. The environment variable will starting with $ and following with alphabet (a-zA-Z0-9)

**Result:**

```c
echo, testing, , "path/home/araiva", 'single', &&, ls, src*, |, wc, -l
```

#### 2.4 Grouping command table

After we parsed token, We will group all token to command table that are splited by operator or redirect metacharacter.
The metacharater are handled

- Pipe `|`
- Or `||` _(bonus part)_
- And `&&` _(bonus part)_
- And `&&` _(bonus part)_

#### Example

This table will show the example every input pattern. You can learn parsing token step by step.

##### _No Quote token_

| Input               | Splited                              | Parsed                             |
| :------------------ | :----------------------------------- | :--------------------------------- |
| `echo x`            | { `echo`, `x` }                      | { `echo`, `x` }                    |
| `echo $HOME`        | { `echo`, `$HOME` }                  | { `echo`, `/users/araiva` }        |
| `echo t*`           | { `echo`, `t*` }                     | { `echo`, `test`, `ttt`, `t` }     |
| `echo x \| echo y`  | { `echo`, `x`, `\|`, `echo`, `y` }   | { `echo`, `x`, `\|`, `echo`, `y` } |
| `echo x \|\|echo y` | { `echo`, `x`, `\|`, `echo`, `y` }   | { `echo`, `x`, `\|`, `echo`, `y` } |
| `echo x&& echo y`   | { `echo`, `x`, `&&`, `echo`, `y` }   | { `echo`, `x`, `&&`, `echo`, `y` } |
| `echo x>>echo y`    | { `echo`, `x`, `>>`, `echo`, `y` }   | { `echo`, `x`, `>>`, `echo`, `y` } |

##### _Single Quote token_

| Input                 | Splited                                | Parsed                               |
| :-------------------- | :------------------------------------- | :----------------------------------- |
| `echo 'x'`            | { `echo`, `'x'` }                      | { `echo`, `x` }                      |
| `echo ' "x" '`        | { `echo`, `' "x" '` }                  | { `echo`, ` "x" ` }                  |
| `echo '$HOME'`        | { `echo`, `'$HOME'` }                  | { `echo`, `t*` }                     |
| `echo 't*'`           | { `echo`, `'t*'` }                     | { `echo`, `$HOME` }                  |
| `echo 'x' \|\|echo y` | { `echo`, `'x'`, `\|\|`, `echo`, `y` } | { `echo`, `x`, `\|\|`, `echo`, `y` } |
| `echo 'x'&& echo y`   | { `echo`, `'x'`, `&&`, `echo`, `y` }   | { `echo`, `x`, `&&`, `echo`, `y` }   |
| `echo 'x'>>echo y`    | { `echo`, `'x'`, `>>`, `echo`, `y` }   | { `echo`, `x`, `>>`, `echo`, `y` }   |

##### _Double Quote token_

| Input                 | Splited                                | Parsed                               |
| :-------------------- | :------------------------------------- | :----------------------------------- |
| `echo "x"`            | { `echo`, `"x"` }                      | { `echo`, `x` }                      |
| `echo " 'x' "`        | { `echo`, `" 'x' "` }                  | { `echo`, ` 'x' ` }                  |
| `echo "$HOME"`        | { `echo`, `"$HOME"` }                  | { `echo`, `/users/araiva` }          |
| `echo "t*"`           | { `echo`, `"t*"` }                     | { `echo`, `test`, `ttt`, `t` }       |
| `echo "x" \|\|echo y` | { `echo`, `"x"`, `\|\|`, `echo`, `y` } | { `echo`, `x`, `\|\|`, `echo`, `y` } |
| `echo "x"&& echo y`   | { `echo`, `"x"`, `&&`, `echo`, `y` }   | { `echo`, `x`, `&&`, `echo`, `y` }   |
| `echo "x">>echo y`    | { `echo`, `"x"`, `>>`, `echo`, `y` }   | { `echo`, `x`, `>>`, `echo`, `y` }   |

### PART 3: Command Execution

--

## more resource

- [adrienblanc]
- [paulahemsi]

<!-- MARKDOWN LINKS & IMAGES -->
[Shell Command Language by POS]: https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html
[Write your own shell]: https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf
[Bash Reference Manual by GNU]: https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Shell-Operation
[Signal Number]: https://pubs.opengroup.org/onlinepubs/009696799/basedefs/signal.h.html
[adrienblanc]: https://adrienblanc.com/projects/42/minishell
[paulahemsi]: https://github.com/paulahemsi/minishell