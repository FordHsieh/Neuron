/******************************************************************************/
/*    Neuron header file -- Neuron.cpp                                        */
/*                                                                            */
/*    Description --                                                          */
/*            Back-propagation network class v1.0                             */
/*									      */
/*    Revise history --                                                       */
/*               programing by deda ford                    2004.02.27        */
/******************************************************************************/
/* C/C++ standard libary */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>

/* class */
#include "BPN.h"
/* namespace */
using namespace std;

struct LotoNumber
{
	int	IDNumber[24];	// 0/1
	int	Number1;
	int	Number2;
	int	Number3;
	int	Number4;
	int	Number5;
	int	Number6;
	int	SpecNumber;
};

int main (int argc, char **argv)
{
	
	BPN myBPN;			// create BPN object
	vector <LotoNumber> number;	// set loto number
	LotoNumber	    BufferNum;	// ...
	string	Buffer;			// string buffer
	string	IDBuffer;		// ID buffer
	double eta;
	/* this is for set input file */
	double		Input[108];
	double		Output[42];
//	char		ArguBuff;

	// set running mode
	if (argc == 2)
	{
		if (strcmp (argv[1], "DEBUG") == 0) myBPN.set_DebugMode ();
		else
			eta = atof(argv[1]);
	}

	// created file object ...
	fstream file;
	file.open ("loto1.txt", ios::in);

	/* read loto file first line */
	while (file >> IDBuffer >> Buffer 
		    >> BufferNum.Number1
		    >> BufferNum.Number2
		    >> BufferNum.Number3
		    >> BufferNum.Number4
		    >> BufferNum.Number5
		    >> BufferNum.Number6
		    >> BufferNum.SpecNumber
		    >> Buffer)
	{
		for (int IDCount = 0 ; IDCount < 24 ; IDCount++)
		{
			if (IDBuffer[IDCount] == '0') BufferNum.IDNumber[IDCount] = 0;
			if (IDBuffer[IDCount] == '1') BufferNum.IDNumber[IDCount] = 1;
			// cout << "IDNumber = " << BufferNum.IDNumber[IDCount] << endl;
		}
		number.push_back(BufferNum);
		cout << setw(25) << IDBuffer << " "
		     << setw(4) << BufferNum.Number1 << " "
		     << setw(4) << BufferNum.Number2 << " "
		     << setw(4) << BufferNum.Number3 << " "
		     << setw(4) << BufferNum.Number4 << " "
		     << setw(4) << BufferNum.Number5 << " "
		     << setw(4) << BufferNum.Number6 << " "
		     << setw(4) << BufferNum.SpecNumber << endl;
	}

	cout << "count = " << number.size () << endl;

	// set input argument
	myBPN.set_argument (108, 75, 42);
	// input value
	bool trainPass = false;
	int TotalNumber = number.size();
	int count = 0;
	int i;
	int GotoCount = 50000;		// for test
	char strOut[100];		// for test

	myBPN.set_eta (eta);
	for (int j = 0 ; j < GotoCount ; j++)
	{		
		for (count = 0 ; count < TotalNumber-1 ; count++)
		{
			// set input zero
			for (i = 0 ; i < 108 ; i++)
			{
				Input[i] = 0.0;
			}
			for (i = 0 ; i < 42 ; i++)
			{
				Output[i] = 0.0;
			}
			// set input number enable
			for (i = 0 ; i < 24 ; i++)
			{
				Input[i] = (double)number[count].IDNumber[i];
				// cout << "Input[" << i << "] = " << Input[i] << endl;
			}
			Input[number[count].Number1+23] = 1.0;
			Input[number[count].Number2+23] = 1.0;
			Input[number[count].Number3+23] = 1.0;
			Input[number[count].Number4+23] = 1.0;
			Input[number[count].Number5+23] = 1.0;
			Input[number[count].Number6+23] = 1.0;
			Input[number[count].SpecNumber+65] = 1.0;

			// set output number
			Output[number[count+1].Number1-1] = 1.0;
			Output[number[count+1].Number2-1] = 1.0;
			Output[number[count+1].Number3-1] = 1.0;
			Output[number[count+1].Number4-1] = 1.0;
			Output[number[count+1].Number5-1] = 1.0;
			Output[number[count+1].Number6-1] = 1.0;
			Output[number[count+1].SpecNumber-1] = 1.0;

			myBPN.set_INvalue (Input);
			myBPN.set_OUTtarget (Output);
			myBPN.training ();
		}

		// show dWeiAvg value
		cout << "myBPN.get_dWeiAvg#" << j << " = " 
		     << myBPN.get_dWeiAvg () << endl;
		if ((fabs (myBPN.get_dWeiAvg ()) < 0.000000001) && 
			(fabs (myBPN.get_YAvg () < 0.00001))) trainPass = true;
		{// set max/min dWeiAvg value
			double dWeiAvgBuffer;
			dWeiAvgBuffer = myBPN.get_dWeiAvg ();
			// set first max/min value
			if (j == 0)
			{
				myBPN.dWeiAvgMax() = dWeiAvgBuffer;
				myBPN.dWeiAvgMin() = dWeiAvgBuffer;
			}
			// set max value
			if (dWeiAvgBuffer > myBPN.dWeiAvgMax ())
			{
				myBPN.dWeiAvgMax() = dWeiAvgBuffer;
			}
			// set mix value
			if (dWeiAvgBuffer < myBPN.dWeiAvgMin ())
			{
				myBPN.dWeiAvgMin() = dWeiAvgBuffer;
			}
		}

		if (j % 30 == 0)
		{
			// set input zero
			for (i = 0 ; i < 108 ; i++)
			{
				Input[i] = 0.0;
			}
			for (i = 0 ; i < 24 ; i++)
			{
				Input[i] = (double)number[TotalNumber-1].IDNumber[i];
				// cout << "Input[" << i << "] = " << Input[i] << endl;
			}
			Input[number[TotalNumber-1].Number1+23] = 1.0;
			Input[number[TotalNumber-1].Number2+23] = 1.0;
			Input[number[TotalNumber-1].Number3+23] = 1.0;
			Input[number[TotalNumber-1].Number4+23] = 1.0;
			Input[number[TotalNumber-1].Number5+23] = 1.0;
			Input[number[TotalNumber-1].Number6+23] = 1.0;
			Input[number[TotalNumber-1].SpecNumber+65] = 1.0;

			myBPN.set_INvalue (Input);
			fstream		Outfile;
			sprintf (strOut, "./LotoReport/recollect%d_%4lf.txt",j,eta);
			Outfile.open (strOut, ios::out);
			if (!Outfile)
			{
				cout << "file can't open ..." << endl;
				return 0;
			}

			Outfile << "¾Ç²ß¦¸¼Æ = " << j <<", eta = " 
				<< eta << endl;
			Outfile << number[TotalNumber-1].Number1 << " "
				<< number[TotalNumber-1].Number2 << " "
				<< number[TotalNumber-1].Number3 << " "
				<< number[TotalNumber-1].Number4 << " "
				<< number[TotalNumber-1].Number5 << " "
				<< number[TotalNumber-1].Number6 << " "
				<< number[TotalNumber-1].SpecNumber << endl;

			myBPN.recollect (Outfile);

			Outfile.close();

			// show Wei range
			cout << "myBPN.get_dWeiAvgRange = " 
			     << myBPN.get_dWeiAvgRange () << endl;
		}

		// save parameter
		fstream fileOUT;
		fileOUT.open ("argument.txt", ios::out);
		myBPN.save_Parameter (fileOUT);
	}

	return 0;
}
