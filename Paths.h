#ifndef PATHS_H
#define PATHS_H

#include <qstring.h>

struct Paths
{
	QString execDir = nullptr;
	QString mat = nullptr;
	QString par = nullptr;
	QString home = nullptr;
	QString proj = nullptr;
	QString rmp = nullptr;
	QString tab = nullptr;
	QString lay = nullptr;
	bool electron = true;
	bool photon = true;
};


#endif