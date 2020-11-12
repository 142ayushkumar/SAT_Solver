An implemenation of DPLL Algorithm  

Course: [Artificial Intelligence: Foundations and Applications AI61005](http://www.facweb.iitkgp.ac.in/~arijit/2020_AIFA_AUTUMN/)


Input Format is taken from: https://www.cs.ubc.ca/~hoos/SATLIB/Benchmarks/SAT/satformat.ps   
For testing, test cases are taken from: https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html  
  

Pseudocode:  
```
dpll(F, literal) {
	1. remove clause containing literal
	2. if (F contains no clauses) return true
	3. shorten clauses containing ~literal
	4. if (F contains empty clause) return false
	5. if (F contains a unit literal or a pure literal(L))
			return dpll(F, L)
	6. choose V in F
	7. if(dpll(F, ~V)) return true
	return dpll(F, V)
}
```


**Installation**  
Prerequisite:  
* C++ compiler which supports C++11 or later  
* For Linux users, C++ compiler can be installed using : `sudo apt install g++`  
  
  
Compilation:
`main.cpp` contains all the code
`cd src`
`g++ -std=c++11 main.cpp -o main`

To run:  
`./main`  
  
Tests:  
* 2 scripts are there in `tests` folder to check for all the cases provided in `testcases` folder. There are 2 folders named `sat` and `unsat`, which contains some tests downloaded from the site mentioned above(tests are not pushed in this repo)

`cd tests`
`chmod +x check_sat.sh`
`chmod +x check_unsat.sh`
`./check_sat.sh`
`./check_unsat.sh`

