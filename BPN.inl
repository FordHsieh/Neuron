/******************************************************************************/
/*    Neuron header file -- BPN.inl                                           */
/*                                                                            */
/*    Description --                                                          */
/*            Back-propagation network class v1.0                             */
/*									      */
/*    Revise history --                                                       */
/*               programing by deda ford                    2004.02.27        */
/******************************************************************************/

/* BPN class member function -- H */
inline double BPN::H (const double NetVal)
{
	return (1 / ( 1 + exp(-NetVal)));
}
