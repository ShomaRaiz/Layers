#ifndef PYTHON_BINDS_H
#define PYTHON_BINDS_H

#include "Paths.h"

namespace PythonBinds 
{
	void start_interpreteter();
	void finalize_interpreteter();
	int calcDistribution(Paths& paths);
	int getDistribution(Paths& paths);
	void initiateInterpritater(std::string scriptsPath);
}


#endif
