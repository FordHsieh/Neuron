/******************************************************************************/
/*    Neuron header file -- BPN.cpp                                           */
/*                                                                            */
/*    Description --                                                          */
/*            Back-propagation network class v1.0                             */
/*									      */
/*    Revise history --                                                       */
/*               programing by deda ford                    2004.02.27        */
/******************************************************************************/
#include "BPN.h"

/* BPN class member function -- constructor */
BPN::BPN (void)
{
	// set default constructor
	neuron		= NULL;
	neuron_OUT	= NULL;
	eta		= 10;
	Target		= NULL;

	INPUT		= 0;
	NLAYER		= 0;
	OUTPUT		= 0;
	runMode		= NORMAL;

	dWeiAvgMIN	= 0.0;
	dWeiAvgMAX	= 0.0;	

	return;
}

/* BPN class member function -- set_argument */
void	BPN::set_argument (const int INPut, const int Nlayer, const int OUTPut)
{
	//               Nlayer
	//           |<--------->|
	//        --->O    O    O    
	//        --->O    O    O     O --->
	// INPut  --->O    O    O     O ---> OUTPut
	//        --->O    O    O     O --->
	//        --->O    O    O     O --->
	//        --->O    O    O

	// check the INPut more than two input number
	if (INPut <= 2)
	{
		cout << "Invalid input value number "
		     << "(more than two input) ... " << endl;
		return;
	}
	// check the Nlayer more than one input number
	if (Nlayer <= 1)
	{
		cout << "Invalid Nlayer number "
		     << "(more than one Nlayer) ... " << endl;
		return;
	}
	// check the INPut and INvalue is the same
	// set rand number
	srand( (unsigned)time(NULL));

	// set constructor -- INPUT, NLAYER, OUTPUT
	{
		INPUT  = INPut;
		NLAYER = Nlayer;
		OUTPUT = OUTPut;

		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "Number input  = " << INPUT  << endl;
			fileHandler << "Number layer  = " << NLAYER << endl;
			fileHandler << "Number output = " << OUTPUT << endl;
		}
	}

	// set constructor -- number of input vlaue and layer
	{
		/* localed number of input value memory */
		neuron = new AIneuron *[INPut];

		/* localed number of layer memory */
		for (int i = 0 ; i < INPut ; i++)
		{
			neuron[i] = new AIneuron [Nlayer];
		}

		/* localed neuron_OUT and Target memory */
		neuron_OUT = new AIneuron [OUTPut];

		/* localed Target memory */
		Target = new double [OUTPut];
	}

	// localed neuron of Wei, dWei memory
	{
		// localed Nlayer-1 neuron Wei, dWei
		for (int i = 0 ; i < Nlayer-1 ; i++) for 
			(int j = 0 ; j < INPUT ; j++)
		{
			neuron[j][i].Wei  = new double [INPut];
			neuron[j][i].dWei = new double [INPut];
		}

		// localed Nlayer neuron Wei, dWei
		for (int k = 0 ; k < INPut ; k++)
		{
			neuron[k][Nlayer-1].Wei  = new double [OUTPut];
			neuron[k][Nlayer-1].dWei = new double [OUTPut];
		}

		// set neuron_OUT Wei, dWei NULL
		for (int h = 0 ; h < OUTPut ; h++)
		{
			neuron_OUT[h].Wei  = NULL;
			neuron_OUT[h].dWei = NULL;
		}
	}

	// set constructor -- set neuron value
	{
		if (runMode == DEBUG)	// debug mode
		{
			fileHandler << "/********* set neuron value"
				    << "**********/" << endl;
		}
		int i,j,k;
		// set Nlayer thita, dthita, deltay, net value
		for (i = 0 ; i < INPut ; i++) for
			(j = 0 ; j < Nlayer ; j++)
		{
			neuron[i][j].thita  = (rand()/RAND_MAX);  // thita value
			neuron[i][j].dthita = (rand()/RAND_MAX);  // dthita value	
			neuron[i][j].deltay = 0.0;	// deltay value
			neuron[i][j].net    = 0.0;	// net value
			neuron[i][j].Hval   = 0.0;	// Hval value

			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << i << "]["
					    << j << "]" << endl;
			}
		}

		// set neuron_OUT thita, dthita, deltay, net value
		for (k = 0 ; k < OUTPut ; k++)
		{
			neuron_OUT[k].thita  = (rand()/RAND_MAX);  // ..
			neuron_OUT[k].dthita = (rand()/RAND_MAX);  // ..
			neuron_OUT[k].deltay = 0.0;	// ..
			neuron_OUT[k].net    = 0.0;	// ..
			neuron_OUT[k].Hval   = 0.0;	// last out put value
			
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron_OUT[" << k << "]"
					    << endl;
			}
		}

		// set 1~Nlayer-1 neuron Wei, dWei
		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "/********* set Wei, dWei "
				    << "default value **********/" 
				    << endl;
		}
		for (i = 0 ; i < INPut ; i++) for
			(j = 0 ; j < (Nlayer -1) ; j++)
		{
			for (k = 0 ; k < INPut ; k++)
			{
				neuron[i][j].Wei[k]  = (rand()/RAND_MAX);
				neuron[i][j].dWei[k] = (rand()/RAND_MAX);
				if (runMode == DEBUG) // debug mode
				{
					fileHandler << "neuron[" << i
						    << "][" << j << "].Wei["
						    << k << "]" << endl;
				}
			}
		}

		// set Nlayer neuron Wei, dWei
		for (i = 0 ; i < INPut ; i++) for
			(j = 0 ; j < OUTPut ; j++)
		{
			neuron[i][Nlayer -1].Wei[j]  = (rand()/RAND_MAX);
			neuron[i][Nlayer -1].dWei[j] = (rand()/RAND_MAX);
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << i << "]["
					    << Nlayer-1 << "].Wei["
					    << j << "]" << endl;
			}
		}

		// set dafault target value
		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "/*********** set target "
				    << "************/" << endl;
		}
		for (i = 0 ; i < OUTPut ; i++)
		{
			Target[i] = 0.0;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "Target[" << i
					    << "]" << endl;
			}
		}
	}

	return;
}

/* BPN class member function -- set_INvalue */
void	BPN::set_INvalue (const double *INvalue)
{
	for (int i = 0 ; i < INPUT ; i++)
	{
		neuron[i][0].Hval = INvalue[i];
	}
	
	return;
}

/* BPN class member function -- set_OUTtarget */
void	BPN::set_OUTtarget (const double *OUTtarget)
{
	for (int i = 0 ; i < OUTPUT ; i++)
	{
		Target[i] = OUTtarget[i];
	}

	return;
}

/* BPN class member fcuntion -- training */
void	BPN::training (void)
{
	int	i, j, k;
	double	Sum;
	if (runMode == DEBUG) // debug mode
	{
		fileHandler << "/************** processing trainning "
			    << "***************/" << endl;
	}

	// caculate each Nlayer net value
	if (runMode == DEBUG) // debug mode
	{
		fileHandler << "/************ caculate each"
			    << " Nlayer net value **************/"
			    << endl;
	}
	for (i = 0 ; i < NLAYER-1 ; i++) for 
		(k = 0 ; k < INPUT ; k++) 
	{
		Sum = 0; // reset sum
		for (j = 0 ; j < INPUT ; j++)
		{
			Sum = Sum + (neuron[j][i].Hval * 
				     neuron[j][i].Wei[k]);
		}
		neuron[k][i+1].net = 
			Sum - neuron[k][i+1].thita;
		neuron[k][i+1].Hval = H (neuron[k][i+1].net);
		if (runMode == DEBUG) // debug mode
		{
			// net value
			fileHandler << "neuron[" << k << "][" << i+1
				    << "].net = " << neuron[k][i+1].net;
			// H value
			fileHandler << "    Hval = " 
				    << neuron[k][i+1].Hval << endl;
		}
	}
	// caculate out put net value
	for (i = 0 ; i < OUTPUT ; i++)
	{
		Sum = 0; // reset sum
		for (k = 0 ; k < INPUT ; k++)
		{
			Sum = Sum + (neuron[k][NLAYER-1].Hval * 
				     neuron[k][NLAYER-1].Wei[i]);
		}
		neuron_OUT[i].net = 
			Sum - neuron_OUT[i].thita;
		neuron_OUT[i].Hval = H (neuron_OUT[i].net);			
		if (runMode == DEBUG) // debug mode
		{
			// net vlaue
			fileHandler << "neuron_OUT[" << i << "].net = " 
				    << neuron_OUT[i].net;
			// H value
			fileHandler << "     Hval = "
				    << neuron_OUT[i].Hval << endl;
		}
		// caculate out put deltay
		// neuron_OUT[i].deltay = neuron_OUT[i].Hval *
		//	(1.0 - neuron_OUT[i].Hval) *
		//	(Target[i] - neuron_OUT[i].Hval);
		neuron_OUT[i].deltay = (neuron_OUT[i].Hval * 
			(1.0 - neuron_OUT[i].Hval) + 0.01) *
			(Target[i] - neuron_OUT[i].Hval);
	
		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "neuron_OUT[" << i 
				    << "].deltay = " 
				    << neuron_OUT[i].deltay << endl;
		}
	}
	// caculate the NLAYER deltay
	if (runMode == DEBUG) // debug mode
	{
		fileHandler << "/*********** caculate the Nlayer"
			    << " deltay **************/" << endl;
	}
	for (i = 0 ; i < INPUT ; i++)
	{
		Sum = 0.0; // reset sum calue
		for (k = 0 ; k < OUTPUT ; k++)
		{
			Sum = Sum + (neuron[i][NLAYER-1].Wei[k] * 
				     neuron_OUT[k].deltay);
		}
		// neuron[i][NLAYER-1].deltay =
		//	neuron[i][NLAYER-1].Hval *
		//	(1 - neuron[i][NLAYER-1].Hval) * Sum;
		neuron[i][NLAYER-1].deltay = 
			(neuron[i][NLAYER-1].Hval *
			(1 - neuron[i][NLAYER-1].Hval) + 0.01) * Sum;
		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "neuron[" << i << "][" 
				    << NLAYER-1 << "].deltay = "
				    << neuron[i][NLAYER-1].deltay
				    << endl;
		}
	}
	// caculate 1 ~ (NLAYER-1) deltay
	for (i = NLAYER-2 ; i >= 0 ; i--)
	{
		for (k = 0 ; k < INPUT ; k++)
		{
			Sum = 0.0; // reset sum value
			for(j = 0 ; j < INPUT ; j++)
			{
				Sum = Sum + (neuron[k][i].Wei[j] * 
					neuron[j][i+1].deltay);
			}
			// neuron[k][i].deltay = neuron[k][i].Hval *
			//		(1 - neuron[k][i].Hval) * Sum;
			neuron[k][i].deltay = (neuron[k][i].Hval *
					(1 - neuron[k][i].Hval) + 0.01) * Sum;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << k << "][" 
					    << i << "].deltay = "
					    << neuron[k][i].deltay
					    << endl;
			}
		}
	}
	// caculate the NLAYER dWei, dthita
	if (runMode == DEBUG) // debug mode
	{
		fileHandler << "/************** caculate the Nlayer "
			    << "dWei, dthita **************/"
			    << endl;
	}
	// dWei
	for (i = 0 ; i < INPUT ; i++)
	{
		for (k = 0 ; k < OUTPUT ; k++)
		{
			neuron[i][NLAYER-1].dWei[k] = 
				eta * neuron_OUT[k].deltay * 
				neuron[i][NLAYER-1].Hval;
		}

		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "neuron_OUT[" << i
				    << "].dthita = "
				    << neuron_OUT[i].dthita << endl;
		}
	}
	// dthita
	for (i = 0 ; i < OUTPUT ; i++)
	{
		neuron_OUT[i].dthita = -eta * neuron_OUT[i].deltay;
	}
	// caculate the 1 ~ (NLAYER-1) dWei, dthita
	for (i = NLAYER-2 ; i >= 0 ; i--)
	{
		for (k = 0 ; k < INPUT ; k++) 
		{
			for(j = 0 ; j < INPUT ; j++)
			{
				neuron[k][i].dWei[j] = 
					eta * neuron[j][i+1].deltay *
					neuron[k][i].Hval;
			}
			neuron[k][i+1].dthita = -eta *neuron[k][i+1].deltay;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << k << "][" 
					    << i << "].dthita = "
					    << neuron[k][i].dthita
					    << endl;
			}
		}
	}
	// caculate all the new Wei(weight) and thita
	{
		if (runMode == DEBUG) // debug mode
		{
			fileHandler << "/*********** caculate all the "
				    << "new Wei(weight), thita *******"
				    << "*****/" << endl;
		}
		// caculate the out put thita ..
		for (i = 0 ; i < OUTPUT ; i++)
		{
			neuron_OUT[i].thita = neuron_OUT[i].thita + 
					      neuron_OUT[i].dthita;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron_OUT[" << i 
					    << "].thita = "
					    << neuron_OUT[i].thita
					    << endl;
			}
		}
		
		// caculate the NLAYER new Wei(weight) and thita
		for (i = 0 ; i < INPUT ; i++)
		{
			// Wei(weight)
			for(k = 0 ; k < OUTPUT ; k++)
			{
				neuron[i][NLAYER-1].Wei[k] = 
					neuron[i][NLAYER-1].Wei[k] + 
					neuron[i][NLAYER-1].dWei[k];
				if (runMode == DEBUG) // debug mode
				{
					fileHandler << "neuron[" << i 
					   <<"][" << NLAYER-1 
					   << "].Wei[" << k << "] = "
					   << neuron[i][NLAYER-1].Wei[k]
					   << endl;
				}
			}
			// thita
			neuron[i][NLAYER-1].thita = 
				neuron[i][NLAYER-1].thita +
				neuron[i][NLAYER-1].dthita;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << i 
				    << "][" << NLAYER-1 << "].thita = "
				    << neuron[i][NLAYER-1].thita
				    << endl;
			}
		}
		// caculate the 1 ~ (NLAYER-1) new Wei(weight) and thita
		for (i = NLAYER-2 ; i >= 0 ; i--) for 
			(k = 0 ; k < INPUT ; k++)
		{
			// Wei(weight)
			for (j = 0 ; j < INPUT ; j++)
			{
				neuron[k][i].Wei[j] = 
					neuron[k][i].Wei[j] +
					neuron[k][i].dWei[j];
				if (runMode == DEBUG) // debug mode
				{
					fileHandler << "neuron[" 
					    << k << "][" << i 
					    << "].Wei[" << j << "] = "
					    << neuron[k][i].Wei[j]
					    << endl;
				}
			}
			// thita
			neuron[k][i].thita = neuron[k][i].thita +
					neuron[k][i].dthita;
			if (runMode == DEBUG) // debug mode
			{
				fileHandler << "neuron[" << k 
					<< "][" << i << "].thita = "
					<< neuron[k][i].thita
					<< endl;
			}
		
		}
	}
	
	return;
}

/* BPN class member function -- set_runMode */
void	BPN::set_DebugMode (void)
{
	// set running mode
	runMode = DEBUG;
	// set log file fstream
	fileHandler.open ("BPN.log", ios::out|ios::trunc);
	cout << "/*******************************/" << endl;
	cout << "/*       RUN DEBUG MODE        */" << endl;
	cout << "/*******************************/" << endl;

	return;
}

/* BPN class member function -- show_OutHval */
void	BPN::show_OutHval (void)
{
	for (int i = 0 ; i < OUTPUT ; i++)
	{
		cout << neuron_OUT[i].Hval << endl;
	}
	
	return;
}

/* BPN class member function -- save _Parameter */
void	BPN::save_Parameter (fstream &file)
{
	/* save all the neuron parameter to file */
	// write argumemnt of input /nlayer/output number
	file << "INPUT= " << INPUT << " ";
	file << "NLAYER= " << NLAYER << " ";
	file << "OUTPUT= " << OUTPUT << endl;

	int i,j,k;
	// write Nlayer thita
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < NLAYER ; j++)
	{
		file << neuron[i][j].thita << " "; // thita value
	}

	// write neuron_OUT thita
	for (k = 0 ; k < OUTPUT ; k++)
	{
		file << neuron_OUT[k].thita << " ";  // ..
	}

	// write 1~Nlayer-1 neuron Wei
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < (NLAYER -1) ; j++)
	{
		for (k = 0 ; k < INPUT ; k++)
		{
			file << neuron[i][j].Wei[k] << " ";
		}
	}

	// write Nlayer neuron Wei
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < OUTPUT ; j++)
	{
		file << neuron[i][NLAYER -1].Wei[j] << " ";
	}

	return;
}

/* BPN class member function -- read_Parameter */
void	BPN::read_Parameter (fstream &file)
{
	char	StrBuff[30];
	double	id;
	// write argumemnt of input /nlayer/output number
	file >> StrBuff >> id;
	file >> StrBuff >> id;
	file >> StrBuff >> id;

	int i,j,k;
	// write Nlayer thita
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < NLAYER ; j++)
	{
		file >> neuron[i][j].thita; // thita value
	}

	// write neuron_OUT thita
	for (k = 0 ; k < OUTPUT ; k++)
	{
		file >> neuron_OUT[k].thita;  // ..
	}

	// write 1~Nlayer-1 neuron Wei
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < (NLAYER -1) ; j++)
	{
		for (k = 0 ; k < INPUT ; k++)
		{
			file >> neuron[i][j].Wei[k];
		}
	}

	// write Nlayer neuron Wei
	for (i = 0 ; i < INPUT ; i++) for
		(j = 0 ; j < OUTPUT ; j++)
	{
		file >> neuron[i][NLAYER -1].Wei[j];
	}

	return;
}

/* BPN class member function -- recollect */
void	BPN::recollect (fstream &file)
{
	int	i, j ,k;
	double	Sum;
	file << "dWeiAvg max Range = " << get_dWeiAvgRange() << endl;
	// caculate NLAYER net value and H value
	for (i = 0 ; i < NLAYER-1 ; i++) for 
		(j = 0 ; j < INPUT ; j++)
	{
		Sum = 0.0;	// reset sum value
		for (k = 0 ; k < INPUT ; k++)
		{
			Sum = Sum + (neuron[k][i].Hval * 
				     neuron[k][i].Wei[j]);
		}
		// net value
		neuron[j][i+1].net = Sum - neuron[j][i+1].thita;
		// H value
		neuron[j][i+1].Hval = H (neuron[j][i+1].net);
	}

	// caculate OUT put net and H value
	for (i = 0 ; i < OUTPUT ; i++)
	{
		Sum = 0.0; // reset sum value
		for (j = 0 ; j < INPUT ; j++)
		{
			Sum = Sum + (neuron[j][NLAYER-1].Hval * 
				     neuron[j][NLAYER-1].Wei[i]);
		}
		// net value
		neuron_OUT[i].net = Sum - neuron_OUT[i].thita;
		// H(Y) value
		neuron_OUT[i].Hval = H (neuron_OUT[i].net);

		/* test value */
		cout << "neuron_OUT[" << i+1 << "] = " 
		     << neuron_OUT[i].Hval << endl;
		file << "neuron_OUT[" << i+1 << "] = " 
		     << neuron_OUT[i].Hval << endl;
	}

	return;
}

/* BPN class member function -- set_eta */
void	BPN::set_eta (const double etaVal)
{
	eta = etaVal;
	return;
}

/* BPN class member function -- get_dWeiAvg */
double	BPN::get_dWeiAvg (void)
{
	int i, j, k, count =0;
	double Sum =0;
	// sum 1~(NLAYER-1) neuron dWei value
	for (i = 0 ; i < NLAYER-1 ; i++) for
		(j = 0 ; j < INPUT ; j++)
	{
		// sum each neuron dWei value
		for (k = 0 ; k < INPUT ; k++)
		{
			Sum = Sum + neuron[j][i].dWei[k];
			count++;
		}
	}
	// sum NALYER neuron dWei value
	for (i = 0 ; i < INPUT; i++) for
		(j = 0 ; j < OUTPUT ; j++)
	{
		Sum = Sum + neuron[i][NLAYER-1].dWei[j];
		count++;
	}

	// cout << "Sum = " << Sum << " count = " << count << endl;

	return Sum/(double)count;
}

/* BPN class member function -- get_YAvg */
double	BPN::get_YAvg (void)
{
	int	i;
	double	Sum =0;
	for (i = 0 ; i < OUTPUT ; i++)
	{
		Sum = Sum + (Target[i] - neuron_OUT[i].Hval);
	}

	//cout << "Sum = " << Sum << endl;
	return Sum/(double)OUTPUT;
}

/* BPN class member function -- get_dWeiAvgRange */
double	BPN::get_dWeiAvgRange (void)
{
	return (dWeiAvgMAX - dWeiAvgMIN);
}

/* BPN class member function -- dWeiAvgMIN  */
double & BPN::dWeiAvgMin (void)
{
	return dWeiAvgMIN;
}

/* BPN class member function -- dWeiAvgMAX */
double	& BPN::dWeiAvgMax (void)
{
	return dWeiAvgMAX;
}