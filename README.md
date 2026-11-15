<div align="center">

# Snake — Neural Network & Genetic Algorithm Experiment

**A C++ / SDL2 game environment for experimenting with evolutionary neural-network agents.**

![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![Graphics](https://img.shields.io/badge/graphics-SDL2-1f6feb)
![AI](https://img.shields.io/badge/AI-neural%20networks%20%2B%20genetic%20algorithms-6f42c1)
![Status](https://img.shields.io/badge/status-experiment-lightgrey)

</div>

## Overview

Snake is a C++ implementation of the classic Snake game, used as an experimental environment for a neural network combined with a genetic algorithm.

Rather than treating the game only as a player-controlled arcade project, the repository explores how agents can be trained through repeated simulations and evolutionary selection.

The original project notes describe an experiment using 300 snakes per generation and a mutation rate of 0.8%. These are recorded experiment settings, not a standardized benchmark.

## Experiment outline

1. Run a population of Snake agents in the game environment.
2. Evaluate their behavior through the simulation.
3. Apply the genetic-algorithm approach to produce a new population.
4. Repeat the process across generations.

Detailed fitness definitions, selection strategy, and training reproducibility should be confirmed from the source before interpreting results.

## Technology stack

- C++
- SDL2
- Artificial neural networks
- Genetic algorithms

## Repository contents

```text
.
├── main.cpp
├── include/
├── model
├── comp.sh
├── libSDL2.so
└── libSDL2_image.so
```

## Build and run

### Requirements

- C++ compiler
- SDL2 and SDL2_image runtime/development support
- Linux environment compatible with the included script

### Compile

```bash
chmod +x comp.sh
./comp.sh
```

Review the script and shared-library requirements before running it on a different Linux distribution.

### Run

Launch the executable produced by the script. The output filename and arguments are defined by `comp.sh`.

## Reproducibility notes

For meaningful comparisons, record the population size, mutation rate, fitness function, selection method, and random seed. The README only repeats the original experiment settings and does not claim independently reproduced performance.

## Project status

AI/game-development experiment. Further optimization, documentation, and controlled evaluation remain possible.

## License

No license file was identified in the repository. Contact the author before redistributing or reusing this project.
