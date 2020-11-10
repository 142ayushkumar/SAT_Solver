#include <bits/stdc++.h>

using namespace std;


// struct cmp {
//     bool operator() (set<int> a, set<int> b) const {
//         return (a.size() < b.size());
//     }
// };



int n_variables, n_clauses;
map<int, int> curr_assignment;

bool dpll(vector<set<int>>& clauses, vector<bool>& already_true, int set_literal=0)
{
	vector<int> store_index;
	vector<int> turned_true;
	// step 1 and 3 from pseudocode, 4 is also included in the elseif condition
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
			else if(clauses[i].find(-set_literal) != clauses[i].end())
			{
				// put this in store_index and remove this literal from this clause
				store_index.push_back(-i);
				clauses[i].erase(-set_literal);
				if(clauses[i].size() == 0) // it must be true for sat otherwise this is unsat for assigned value yet
					return false;
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
			return dpll(clauses, already_true, unit_literal);
		}
	}
	// remaning part of step 5, checking for pure literals
	for(int j=1;j<=n_variables;j++)
	{
		bool found = false, positive = true;
		for(int i=0;i<(int)clauses.size();i++)
		{
			if(already_true[i]) continue;
			if(clauses[i].find(j) != clauses[i].end())
			{
				if(!found)
				{
					found= true;
				}
				else if(!positive)
				{
					found = false;
					break;
				}
			}
			if(clauses[i].find(-j) != clauses[i].end())
			{
				if(!found)
				{
					found = true;
					positive = false;
				}
				else if(positive)
				{
					found = false;
					break;
				}
			}
		}
		// if found is false => either impure literal or not found
		if(found)
			return dpll(clauses, already_true, j);
	}

	// step 6, choosing a clause and assigning it
	int unassigned = 0;
	bool satisfied_all = true;
	for(int i=1;i<=n_variables;i++)
	{
		if(clauses[i].size())
		{
			satisfied_all = false;
		}
		if(curr_assignment.find(i) == curr_assignment.end())
		{
			unassigned = i;
			break;
		}
	}
	if(unassigned == 0)
	{
		if(satisfied_all) return true;
		else 
		{
			return false;
		}
	}
	if(dpll(clauses, already_true, -unassigned))
	{
		return true;
	}
	else 
	{
		// remove the current assignmeent
		curr_assignment.erase(set_literal);
		// turn all those assignment turned false by this
		for(int i:turned_true)
			already_true[i] = false;
		// add this literal in all the clauses in which true was assigned
		for(int i:store_index)
		{
			if(i < 0)
				clauses[-i].insert(-set_literal);
			else
				clauses[i].insert(set_literal);
		}
		return dpll(clauses, already_true, unassigned);
	}
}

int main()
{
	cout << "Enter number of variables and number of clauses\n";
	cin >> n_variables >> n_clauses;

	vector<set<int>> clauses;
	for(int i=0;i<n_clauses;i++)
	{
		set<int> curr;
		int tmp;
		while(1)
		{
			cin >> tmp;
			if(tmp == 0) break;
			curr.insert(tmp);
		}
		clauses.push_back(curr);
	}
	vector<bool> already_true(n_clauses, 0);
	bool found = dpll(clauses, already_true);
	if(found)
	{
		cout << "SAT\n";
		cout << "Curr_asignment size is " << curr_assignment.size() << "\n";
		for(auto i:curr_assignment)
			cout << i.first << " " << i.second << "\n";
		cout <<"\n";
	}
	else cout << "UNSAT\n";
}
