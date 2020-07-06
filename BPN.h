/******************************************************************************/
/*    Neuron header file -- BPN.h                                             */
/*                                                                            */
/*    Description --                                                          */
/*            Back-propagation network class v1.0                             */
/*                                                                            */
/*    Revise history --                                                       */
/*               programming by deda ford                    2004.02.27       */
/*               programming by deda ford second edition     2004.09.10       */
/******************************************************************************/
#ifndef BPN_H
#define BPN_H

/* Stander C++ lib */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include <iomanip>

/* namespace */
using namespace std;

/* neuron struct */
struct AIneuron
{
	double	net;		// net value
	double	Hval;		// H value
	double	deltay;		// deltay value
	double	thita;		// thita value
	double	dthita;		// correction of thita
	double	*Wei;		// point of weight value
	double	*dWei;		// point of correction of weight
};

/* enum struct -- for check caculate data */
enum RunMode
{
	NORMAL,		// normal mode
	DEBUG		// debug mode
};

/* BPN class */
class BPN
{
private:
	AIneuron	**neuron;	// point of struct neuron (N layer)
	AIneuron	*neuron_OUT;	// point of struct neuron output value
	double		eta;		// ...
	double		*Target;	// target value

	int		INPUT;		// number of input value
	int		NLAYER;		// number of hidden layer
	int		OUTPUT;		// number of output value

	double		dWeiAvgMIN;	// WeiAvg min value
	double		dWeiAvgMAX;	// WeiAvg max value	
public:
	RunMode		runMode;	// running mode
	fstream		fileHandler;	// write log file

public:	// default constructor
	BPN (void);

	/* INPut  ---- number of input value        */
	/* Nlayer ---- number of hidden layer       */
	/* OUTPut ---- number of output value       */
	void	set_argument (const int INPut, const int Nlayer, 
			      const int OUTPut);

public:
	// set input train value
	void	set_INvalue (const double *INvalue);
	// set out put target value
	void	set_OUTtarget (const double *OUTtarget);
	// training the BPN net..
	void	training (void);
	// set running mode
	void	set_DebugMode (void);
	// show out put neuron value
	void	show_OutHval (void);
	// save all the neuron parameter(Wei/theta)
	void	save_Parameter (fstream &file);
	// read all the neuron parameter(Wei/theta)
	void	read_Parameter (fstream &file);
	// recollect the training
	void	recollect (fstream &file);
	// set eta value
	void	set_eta (const double etaVal);
	// get all wei average
	double	get_dWeiAvg (void);
	// get all Y out average
	double	get_YAvg (void);
	// get Wei average max range
	double	get_dWeiAvgRange (void);
	// set Wei average min value
	double	& dWeiAvgMin (void);
	// set Wei average max value
	double	& dWeiAvgMax (void);

public:
	// H function
	inline double	H (const double NetVal);
	// delta
//	inline double	delta (const double _H, );
};

#include "BPN.inl"

#endif
