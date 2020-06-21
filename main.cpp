#include <iostream>
#include <vector>
#include "Variable.h"
#include "ConstraintsSolver.h"

void solve(std::vector<std::vector<int>> sudoku) {

    csl::ConstraintsSolver<csl::Variable<int>> solver;
    solver.print(true);

    csl::Variable<int>* vars[9][9];

    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++) {
            if (sudoku[i][j])
                vars[i][j] = new csl::Variable<int>({sudoku[i][j]}, "v"+std::to_string(i)+std::to_string(j));
            else
                vars[i][j] = new csl::Variable<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}, "v"+std::to_string(i)+std::to_string(j));
            solver.add_variable(vars[i][j]);
        }

    // same row
    for (int i=0; i<9; i++)
        for(int a=0; a<9; a++)
            for(int b=a+1; b<9; b++)
                solver.add_constraint(vars[i][a], vars[i][b]);

    // same column
    for (int i=0; i<9; i++)
        for(int a=0; a<9; a++)
            for(int b=a+1; b<9; b++)
                solver.add_constraint(vars[a][i], vars[b][i]);

    //same square
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            for (int a=(i/3)*3; a<(i/3)*3+3; a++)
                for (int b=(j/3)*3; b<(j/3)*3+3; b++)
                    if (a != i and b!=j)
                        solver.add_constraint(vars[i][j], vars[a][b]);

    if(solver.solve_any())
        for (int i=0; i<9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << vars[i][j]->unique_value().value() << " ";
            }
            std::cout<<"\n";
        }
    else
        std::cout<<"no solution\n";


}

int main() {

    std::vector<std::vector<int>> sudoku_easy = {
        {0, 0, 0, 0, 4, 0, 0, 0, 0},
        {0, 0, 4, 2, 0, 3, 5, 0, 0},
        {5, 0, 0, 1, 0, 7, 0, 0, 8},
        {0, 5, 1, 0, 6, 0, 8, 3, 0},
        {4, 0, 0, 8, 0, 2, 0, 0, 9},
        {0, 9, 6, 0, 7, 0, 4, 2, 0},
        {6, 0, 0, 7, 0, 1, 0, 0, 2},
        {0, 0, 9, 6, 0, 4, 1, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 0, 0}
    };

    std::vector<std::vector<int>> sudoku_hard = {
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 7, 0, 6, 0, 3, 0, 0, 8},
        {0, 3, 6, 0, 5, 0, 2, 7, 0},
        {0, 6, 0, 9, 0, 0, 0, 1, 0},
        {0, 0, 9, 0, 0, 0, 4, 0, 0},
        {0, 1, 0, 0, 0, 5, 0, 2, 0},
        {0, 0, 7, 0, 2, 0, 3, 0, 0},
        {0, 0, 2, 7, 0, 6, 5, 4, 0},
        {0, 5, 0, 0, 0, 0, 0, 9, 2}
    };

    std::vector<std::vector<int>> sudoku_multi = {
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 7, 0, 6, 0, 3, 0, 0, 8},
        {0, 3, 6, 0, 5, 0, 2, 7, 0},
        {0, 6, 0, 9, 0, 0, 0, 1, 0},
        {0, 0, 9, 0, 0, 0, 4, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 2, 0},
        {0, 0, 7, 0, 2, 0, 3, 0, 0},
        {0, 0, 2, 7, 0, 6, 5, 4, 0},
        {0, 5, 0, 0, 0, 0, 0, 0, 0}
    };

    std::vector<std::vector<int>> sudoku_no_sol = {
        {9, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 7, 0, 6, 0, 3, 0, 0, 8},
        {0, 3, 6, 0, 5, 0, 2, 7, 0},
        {0, 6, 0, 9, 0, 0, 0, 1, 0},
        {0, 0, 9, 0, 0, 0, 4, 0, 0},
        {0, 1, 0, 0, 0, 5, 0, 2, 0},
        {0, 0, 7, 0, 2, 0, 3, 0, 0},
        {0, 0, 2, 7, 0, 6, 5, 4, 0},
        {0, 5, 0, 1, 0, 0, 0, 9, 2}
    };

    solve(sudoku_easy);

    solve(sudoku_hard);

    solve(sudoku_multi);

    solve(sudoku_no_sol);

}
