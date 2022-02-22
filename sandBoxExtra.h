#pragma once
#include "External.h"
#include "ClassIncludes.h"
#include "gurobi_c++.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
using namespace std;


//ofstream myfile;
//myfile.close ();
//ofstream myfile; //Defining file name
//myfile.open ("final.qlp"); //Opening the file

using Matrix = std::vector<std::vector<double>>;
using Vector = std::vector<double>;
/* reads the coefficient matrix A, the objective coefficients c
   and the constraints rhs b */
    std::tuple<Matrix, Vector, Vector> read_Abc(GRBModel& model) {
        // number of variables and number of constraints
        std::size_t n = model.get(GRB_IntAttr_NumVars);
        std::size_t m = model.get(GRB_IntAttr_NumConstrs);

        // allocate space for A, b, c
        Matrix A(n, Vector(m, 0.0));
        Vector b(m, 0.0);
        Vector c(n, 0.0);

        // Read the objective coefficients, i.e. the vector c
        auto obj_expr = model.getObjective().getLinExpr();
        for (std::size_t j = 0; j < n; ++j) {
            c[j] = obj_expr.getCoeff(j);
        }
        // Read the coefficient matrix A and the rhs vector b
        for (std::size_t i = 0; i < m; ++i) {
            auto con = model.getConstr(i);
            auto row = model.getRow(model.getConstr(i));
            for (std::size_t j = 0; j < n; j++) {
                A[i][j] = row.getCoeff(j);
            }
            b[i] = con.get(GRB_DoubleAttr_RHS);
        }
        return {A, b, c};
    }
    double fileReader(){
        GRBEnv env     = GRBEnv();
        GRBModel model = GRBModel(env,"/home/mert/CLionProjects/motif-gurobi/Small_Investment_Model.lp");
        //GRBModel model = GRBModel(env,"/home/mert/CLionProjects/motif-gurobi/Medium_Investment_Model.lp");
        //GRBModel model = GRBModel(env,"/home/mert/CLionProjects/motif-gurobi/Small_Investment_Model_presolved.lp");


        auto [A, b, c] = read_Abc(model);
        // ... your code here. Let's print A, b, c for demonstration purposes ...
        for (std::size_t j = 0; j < c.size(); ++j) {
            std::cout << "c[" << j << "] = " << c[j] << '\n';
        }

        for (std::size_t i = 0; i < A.size(); ++i) {
            for (std::size_t j = 0; j < A[0].size(); j++) {
                std::cout << "A[" << i << "][" << j << "] = " << A[i][j] << '\n';
            }
        }

        for (std::size_t i = 0; i < b.size(); i++) {
            std::cout << "b[" << i << "] = " << b[i] << '\n';
        }
 }
