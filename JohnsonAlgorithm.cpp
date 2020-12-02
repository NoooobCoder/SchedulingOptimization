#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int JohnsonAlgo(double**, int, int, int*);
void get_min(double**, int*, int, int);

int main() {

	ifstream myfile;
	string filename;
	int njobs, nmachines;
	int i, j, flag;
	
	
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

	if (nmachines != 2)
	{
		cout << "FATAL ERROR! nmachines to Johnson Algo != 2. \n";
		return -1;
	}

	int first_avail, last_avail;
	int i, j, m;
	int unscheduled_jobs = njobs;

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
	float m;
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