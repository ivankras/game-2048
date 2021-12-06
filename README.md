# Game: 2048


## How to run
- Compile
```sh
$ g++ -std=c++14 -w -g src/*.cpp src/*.h -o 2048.exe
```

- Execute
```sh
$ ./2048.exe
```

----
## How to play

### Objectives
1) Combine equal numbers by sliding in any direction until getting a 2048
2) Achieve the highest score

### Commands
- a => slide left
- d => slide right
- s => slide down
- w => slide up
- z => undo last move (no more than one consecutive undo is allowed; no undos are allowed if the score is higher than 4000)
- q => quit
