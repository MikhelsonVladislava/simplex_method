# Linear Programming Solver

A educational implementation of the Simplex Method for solving linear programming problems with inequality constraints, featuring a Qt-based graphical interface.

## 📋 Project Overview

This application solves linear programming problems presented in the form of inequality constraints using the classic Simplex Method algorithm.

## ✨ Features

- **Graphical User Interface** built with Qt framework
- **Simplex Method implementation** for solving LP problems
- **Inequality constraint support** (≤, ≥, =)
- **Step-by-step solution display** for educational purposes
- **Result visualization** with optimal values and objective function

## 🚀 Installation

### Prerequisites

- Qt 6.4.1 or higher
- C++17 compatible compiler
- CMake 3.5

### Building from Source

```bash
# Clone the repository
git clone https://github.com/MikhelsonVladislava/simplex_method.git
cd simplex_method

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release
```

## 🔧 Implementation Details

- The code implements the Fraction class (in units.cpp), which represents a common fraction;
- The solution process is iterative and at each iteration uses the states described in the corresponding class;
- The result of the solution is visualized in a special class responsible for view layer;

⚠️ Project Status: Under Active Development
