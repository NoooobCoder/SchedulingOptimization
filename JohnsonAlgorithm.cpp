#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int JohnsonAlgo(double**, int, int, int*);
void get_min(double**, int*, int, int);
int check_condition(double**, int, int);
double** transform(double**, int, int, int);
double get_min1d(double*, int);
double get_max1d(double*, int);
double simulate_schedule(double**, int*, int, int);

int main() {

	ifstream myfile;
	string filename;
	int njobs, nmachines;
	int i, j, flag;
	double m;
	
	
	filename = "data.txt";
	myfile.open(filename);
	if (myfile.is_open())
	{
		myfile >> njobs;
		myfile >> nmachines;
		cout << "njobs=" << njobs << endl;
		cout << "nmachines=" << nmachines << endl;

		int* seq = new int[njobs];
		double** pij = new double*[njobs];
		for (i = 0; i < njobs; i++)
		{
			pij[i] = new double[nmachines];
		}

		cout << "Processing times = \n";
		for (i = 0; i < njobs; i++)
		{
			for (j = 0; j < nmachines; j++)
			{
				myfile >> pij[i][j];
				cout << pij[i][j] << " ";
			}
			cout << "\n";
		}
		myfile.close();
		
		flag = JohnsonAlgo(pij, njobs, nmachines, seq);

		if (flag == 0)
		{
			cout << "Optimal sequence is: \n";
			cout << "[ ";
			for (i = 0; i < njobs; i++)
			{
				cout << seq[i] << " ";
			}
			cout << "] \n";

			m = simulate_schedule(pij, seq, njobs, nmachines);
			cout << "The makespan is ";
			cout << m;
		}
		delete[] pij;
		delete[] seq;
	}

	else
	{
		cout << "Not able to open the file \n";
	}

	return 0;

}

int JohnsonAlgo(double** pij, int njobs, int nmachines, int* schedule){

	int first_avail, last_avail;
	int i, j, m;
	int unscheduled_jobs = njobs;

	if (nmachines != 2)
	{
		if (nmachines == 3)
		{
			int flag;
			int i;
			flag = check_condition(pij, njobs, nmachines);
			if (flag == 0)
			{
				cout << "nmachines = 3; Johnson's algo will give optimal solution\n";
				double** pij_new = new double* [njobs];
				for (i = 0; i < njobs; i++)
				{
					pij_new[i] = new double[2];
				}
				pij_new = transform(pij, njobs, nmachines, 2);
				return JohnsonAlgo(pij_new, njobs, 2, schedule);
			}
			else if(flag == -1)
			{
				cout << "nmachines = 3;The condition is not satisfied! Johnson's algo maynot solve to optimality\n";
				return -1;
			}
			else
			{
				return -2;
			}
		}
		cout << "FATAL ERROR! nmachines to Johnson Algo != 2,3. \n";
		return -3;
	}

	

	first_avail = 0;
	last_avail = njobs - 1;
	int* d = new int[2];

	while (unscheduled_jobs > 0)
	{
		get_min(pij, d, njobs, nmachines);

		if (d[1] == 0)
		{
			schedule[first_avail] = d[0];
			unscheduled_jobs = unscheduled_jobs - 1;
			first_avail = first_avail + 1;
		}
		else
		{
			schedule[last_avail] = d[0];
			unscheduled_jobs = unscheduled_jobs - 1;
			last_avail = last_avail - 1;
		}
	}
	delete[] d;
	return 0;

}

void get_min(double** pij, int* d, int njobs, int nmachines) {

	int i, j;
	double m;
	i = 0;
	j = 0;
	m = 10000.0;
	d[0] = i;
	d[1] = j;

	for ( i = 0; i < njobs; i++)
	{
		for ( j = 0; j < nmachines; j++)
		{
			if (*(*(pij + i) + j) >= 0)
			{
				if (*(*(pij + i) + j) < m)
				{
					m = *(*(pij + i) + j);
					d[0] = i;
					d[1] = j;
				}
			}
		}
	}
	i = d[0];
	pij[i][0] = -1.0;
	pij[i][1] = -1.0;

	return;

}

int check_condition(double** pij, int njobs, int nmachines) {

	if (nmachines != 3)
	{
		cout << "nmachines!=3; Please check input to check_condition\n";
		return -2;
	}

	int i, j;
	double m[3];

	double* pj = new double[njobs];

	for (j = 0; j < nmachines; j++)
	{
		for (i = 0; i < njobs; i++)
		{
			pj[i] = pij[i][j];
		}
		if (j != 1)
		{
			m[j] = get_min1d(pj, njobs);
		}
		else
		{
			m[j] = get_max1d(pj, njobs);
		}
	}

	if (m[0] >= m[1] || m[2] >= m[1])
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

double get_min1d(double* pj, int njobs) {
	int i;
	double m;

	m = pj[0];

	for (i = 0; i < njobs; i++)
	{
		if (m > pj[i])
		{
			m = pj[i];
		}
	}
	return m;
}

double get_max1d(double* pj, int njobs) {
	int i;
	double m;

	m = pj[0];

	for (i = 0; i < njobs; i++)
	{
		if (m < pj[i])
		{
			m = pj[i];
		}
	}
	return m;
}

double** transform(double** pij, int njobs, int nmachines, int together) {

	int i, j, k;
	double s1, s2;

	double** pij_new = new double*[njobs];
	for (i = 0; i < njobs; i++)
	{
		pij_new[i] = new double[2];
	}

	for (i = 0; i < njobs; i++)
	{
		s1 = 0.0;
		s2 = 0.0;
		for ( j = 0; j < together; j++)
		{
			s1 = s1 + pij[i][j];
			s2 = s2 + pij[i][nmachines - 1 - j];
		}
		pij_new[i][0] = s1;
		pij_new[i][1] = s2;
	}
	return pij_new;
}

double simulate_schedule(double** pij, int* seq, int njobs, int nmachines) {

	double makespan;
	int i, j;
	double s;

	double* prev_machine = new double[njobs];
	double* this_machine = new double[njobs];
	
	s = 0.0;
	for (i = 0; i < njobs; i++)
	{
		s = s + pij[seq[i]][0];
		this_machine[i] = s;
	}

	for (j = 1; j < nmachines; j++)
	{
		prev_machine[0] = this_machine[0];
		this_machine[0] = prev_machine[0] + pij[seq[0]][j];
		for (i = 1; i < njobs; i++)
		{
			prev_machine[i] = this_machine[i];
			if (prev_machine[i] + pij[seq[i]][j] >= this_machine[i - 1] + pij[seq[i]][j])
				this_machine[i] = prev_machine[i] + pij[seq[i]][j];
			else this_machine[i] = this_machine[i - 1] + pij[seq[i]][j];
		}
	}

	makespan = this_machine[njobs - 1];

	delete[] prev_machine;
	delete[] this_machine;

	return makespan;

}