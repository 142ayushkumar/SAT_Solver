#include <bits/stdc++.h>

using namespace std;



int n_variables, n_clauses;
map<int, int> curr_assignment;

void clear_assignment(vector<set<int>>& clauses, vector<bool>& already_true, int set_literal, vector<int>& store_index, vector<int>& turned_true)
{
	// remove the effect of current assignmeent
	if(set_literal != 0)
	{
		curr_assignment.erase(abs(set_literal));
		// turn off all those assignment turned on by this assignment
		for(int x:turned_true)
			already_true[x] = false;
		// add this literal in all the clauses in which true was assigned
		for(int x:store_index)
		{
			clauses[x].insert(-set_literal);
		}
	}
}


bool dpll(vector<set<int>>& clauses, vector<bool>& already_true, int set_literal=0)
{
	vector<int> store_index;
	vector<int> turned_true;
	// step 1 and 3 from pseudocode, 4 is also included in the last if condition
	if(set_literal != 0)
	{
		curr_assignment[abs(set_literal)] = set_literal;
		// remove clauses containing literal
		for(int i=0;i<(int)clauses.size();i++)
		{
			if(already_true[i]) continue;
			if(clauses[i].find(set_literal) != clauses[i].end())
			{
				// this makes clause true
				turned_true.push_back(i);
				already_true[i] = 1;
			}
			if(already_true[i]) continue; // if a clause contains both a and ~a
		
			if(clauses[i].find(-set_literal) != clauses[i].end())
			{
				// put this in store_index and remove this literal from this clause
				store_index.push_back(i);
				clauses[i].erase(-set_literal);
				if(clauses[i].size() == 0) // it must be true for sat otherwise this is unsat for assigned value yet
				{
					clear_assignment(clauses, already_true, set_literal, store_index, turned_true);
					return false;
				}
			}
		}
	}
	// if clauses has everything satisfied, return true
	bool all_sat = true;
	for(int i=0;i<(int)already_true.size();i++)
	{
		if(!already_true[i])
			all_sat = false;
	}
	if(all_sat) return true;

	// now step 5, check for unit literal
	int unit_literal = 0;
	for(int i=0;i<(int)clauses.size();i++)
	{
		if(clauses[i].size() == 1 and already_true[i] == false)
		{
			//unit literal
			unit_literal = *clauses[i].begin();
			bool final = dpll(clauses, already_true, unit_literal);
			if(final) return true;
			else 
			{	
				clear_assignment(clauses, already_true, set_literal, store_index, turned_true);
				return false;
			}
		}
	}
	// remaning part of step 5, checking for pure literals
	for(int j=1;j<=n_variables;j++)
	{
		bool positive = false, negative= false;
		if(curr_assignment.find(j) != curr_assignment.end()) continue;
		for(int i=0;i<(int)clauses.size();i++)
		{
			if(already_true[i]) continue;
			if(clauses[i].find(j) != clauses[i].end())
			{
				positive = true;
			}
			if(clauses[i].find(-j) != clauses[i].end())
			{
				negative = true;
			}
		}
		// either of positive and negative should be true for pure literal to be found
		if(positive xor negative)
		{
			int literal = j;
			if(negative) literal = -j;
			bool final = dpll(clauses, already_true, literal);
			if(final)
			{
				return true;
			}
		}
	}

	// step 6, choosing a clause and assigning it
	int unassigned = 0;

	bool satisfied_all = true;
	for(int i=0;i<n_clauses;i++)
	{
		if(already_true[i] == 0)
		{
			satisfied_all = false;
			break;
		}
	}
	if(satisfied_all) return true;

	for(int i=1;i<=n_variables;i++)
	{
		if(curr_assignment.find(i) == curr_assignment.end())
		{
			unassigned = i;
			break;
		}
	}

	if(unassigned == 0) // everything is assigned but not all conditions are satisfied
	{
		clear_assignment(clauses, already_true, set_literal, store_index, turned_true);
		return false;
	}
	if(dpll(clauses, already_true, -unassigned))
	{
		return true;
	}
	else 
	{
		bool final = dpll(clauses, already_true, unassigned);
		if(final) 
		{
			return true;
		}
		else
		{
			clear_assignment(clauses, already_true, set_literal, store_index, turned_true);
			return false;
		}
	}
}

int main()
{
	string s;
	while(1)
	{
		getline(cin, s);
		// before any clause, a line with p cnf ... will appear
		if(s[0] == 'p')
		{
			// problem line
			// p cnf numberOfVariables numberOfClauses
			stringstream ss(s);
			vector<string> v;
			string tmp;
			while(ss >> tmp)
			{
				v.push_back(tmp);
			}
			n_variables = atoi(v[2].c_str());
			n_clauses = atoi(v[3].c_str());
			break;
		}
	}
	vector<set<int>> clauses;
	while(getline(cin, s))
	{
		if(s[0] > '9' and s[0] <= '0' or s[0] == '\n') continue; // not required comment or solution, not required as of now, if 0 is there, it is not a clause just signify end in few test cases and \n for empty lines
		stringstream ss(s); // don't know if this is faster or just using above method of looping ?
		set<int> curr;
		int tmp;
		while(ss >> tmp)
		{
			if(tmp == 0) 
			{
				if(curr.size())
				{
					clauses.push_back(curr);
					curr.clear();
				}
			}
			else 
			{
				curr.insert(tmp);
			}
		}
		
		if(curr.size()) // if not empty
		{
			clauses.push_back(curr);
		}
	}
	assert(clauses.size() == n_clauses);
	vector<bool> already_true(n_clauses, 0);
	bool found = dpll(clauses, already_true);
	if(found)
	{
		cout << "SAT\n";
		for(int i=1;i<=n_variables;i++)
		{
			cout << curr_assignment[i] << " ";
		}
		cout << "\n";
	}
	else cout << "UNSAT\n";

	return 0;
}
