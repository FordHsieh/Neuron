/******************************************************************************/
/*    Neuron header file -- Symbol.h                                          */
/*                                                                            */
/*    Description --                                                          */
/*            Back-propagation network class v1.0                             */
/*                                                                            */
/*    Revise history --                                                       */
/*               programing by deda ford                    2004.02.27        */
/******************************************************************************/
#ifndef Symbol_H
#define Symbol_H

enum RunMode
{
	NORMAL,
	DEBUG
};

extern RunMode 	runMode;

#endif