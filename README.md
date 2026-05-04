# Cshell

A minimal Unix-like shell implemented in C to explore core operating system concepts such as process creation, command execution, and I/O redirection.

---

## ✨ Features

* Execute system commands using `fork()` and `execvp()`
* Built-in commands:

  * `cd` – change directory
  * `history` – show last 10 commands
  * `exit` – terminate shell
* Input/Output redirection:

  * `>` overwrite output
  * `>>` append output
  * `<` read input from file
* Background execution using `&`
* Signal handling (`Ctrl + C` does not terminate shell)

---

## ⚙️ How It Works

1. Shell reads user input
2. Parses command into tokens
3. Uses `fork()` to create a child process
4. Child executes command using `execvp()`
5. Parent waits (or runs in background)

---

## 🛠️ Installation & Usage

```bash
gcc shell.c -o cshell
./cshell
```

---

## 📌 Example Usage

```bash
Cshell> ls
Cshell> cd ..
Cshell> echo hello > file.txt
Cshell> cat < file.txt
Cshell> sleep 5 &
Cshell> history
```

---

## 🧠 Concepts Used

* Process management (`fork`, `waitpid`)
* Program execution (`execvp`)
* File descriptors and redirection (`dup2`)
* Signal handling (`SIGINT`)
* Basic command parsing

---

## 🚧 Limitations

* No support for pipes (`|`)
* Limited parsing (no quotes or advanced syntax)
* Basic tokenizer (space-separated only)

---

## 🚀 Future Improvements

* Add pipe support (`|`)
* Improve command parsing (quotes, escapes)
* Implement tab auto-completion
* Add job control (`fg`, `bg`)

---

## 📄 License

This project is for educational purposes.

