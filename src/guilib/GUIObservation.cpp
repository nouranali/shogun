#include "guilib/GUIObservation.h"
#include "lib/io.h"
#include <string.h>
#include "gui/GUI.h"

CGUIObservation::CGUIObservation(CGUI * gui_): gui(gui_)
{
	alphabet=DNA;
	pos_train_obs=NULL;
	neg_train_obs=NULL;
	pos_test_obs=NULL;
	neg_test_obs=NULL;
	test_obs=NULL;

	test_name=NULL;
	neg_test_name=NULL;
	pos_test_name=NULL;
}

CGUIObservation::~CGUIObservation()
{
	delete pos_train_obs;
	delete neg_train_obs;
	delete pos_test_obs;
	delete neg_test_obs;
	delete test_obs;
	delete neg_test_name;
	delete test_name;
	delete pos_test_name;
}

bool CGUIObservation::load_observations(char* param)
{
	char* input=CIO::skip_spaces(param);
	char filename[1024];
	char target[1024];
	int M=-1;
	int ORDER=1;
	int start=0;
	int width=-1;

//	if (gui->guihmm.get_pos())
//		M=gui->guihmm.get_pos()->get_M();
//	else if (gui->guihmm.get_neg())
//		M=gui->guihmm.get_neg()->get_M();
//	else if (gui->guihmm.get_test())
//		M=gui->guihmm.get_test()->get_M();
//	else
		M=get_alphabet_size();

	if ((sscanf(input, "%s %s %d %d %d", filename, target, &ORDER, &start, &width))>=2)
	{
		FILE* trn_file=fopen(filename, "r");

		if (trn_file)
		{
			if (strcmp(target,"POSTRAIN")==0)
			{
				delete pos_train_obs;
				pos_train_obs= new CObservation(trn_file, POSTRAIN, alphabet, (BYTE)ceil(log((double) M)/log((double) 2)), M, ORDER, start, width);
			}
			else if (strcmp(target,"NEGTRAIN")==0)
			{
				delete neg_train_obs;
				neg_train_obs= new CObservation(trn_file, NEGTRAIN, alphabet, (BYTE)ceil(log((double) M)/log((double) 2)), M, ORDER, start, width);
			}
			else if (strcmp(target,"POSTEST")==0)
			{
				delete pos_test_obs;
				delete pos_test_name;
				pos_test_name=strdup(filename);
				pos_test_obs= new CObservation(trn_file, POSTEST, alphabet, (BYTE)ceil(log((double) M)/log((double) 2)), M, ORDER, start, width);
			}
			else if (strcmp(target,"NEGTEST")==0)
			{
				delete neg_test_obs;
				delete neg_test_name;
				neg_test_name=strdup(filename);
				neg_test_obs= new CObservation(trn_file, NEGTEST, alphabet, (BYTE)ceil(log((double) M)/log((double) 2)), M, ORDER, start, width);
			}
			else if (strcmp(target,"TEST")==0)
			{
				delete test_obs;
				delete test_name;
				test_name=strdup(filename);
				test_obs= new CObservation(trn_file, TEST, alphabet, (BYTE)ceil(log((double) M)/log((double) 2)), M, ORDER, start, width);
			}
			else
				CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");
			fclose(trn_file);
		}
		else
			printf("opening file %s failed\n", filename);
	}
	else
	{
		CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");
		return false;
	}

	return true;
}

CObservation* CGUIObservation::get_obs(char* param)
{
	param=CIO::skip_spaces(param);
	char target[1024];

	if ((sscanf(param, "%s", target))==1)
	{
		if (strcmp(target,"POSTRAIN")==0)
			return pos_train_obs;
		else if (strcmp(target,"NEGTRAIN")==0)
			return neg_train_obs;
		else if (strcmp(target,"POSTEST")==0)
			return pos_test_obs;
		else if (strcmp(target,"NEGTEST")==0)
			return neg_test_obs;
		else if (strcmp(target,"TEST")==0)
			return test_obs;
		else
			CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");
	}
	else
		CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");

	return NULL;
}

bool CGUIObservation::set_max_dim(char* param)
{
	char* input=CIO::skip_spaces(param);
	int dim=-1;
	char target[1024];

	if ((sscanf(input, "%d %s", &dim, target))==2)
	{
		CObservation* obs=NULL;

		if (strcmp(target,"POSTRAIN")==0)
			obs=pos_train_obs;
		else if (strcmp(target,"NEGTRAIN")==0)
			obs=neg_train_obs;
		else if (strcmp(target,"POSTEST")==0)
			obs= pos_test_obs;
		else if (strcmp(target,"NEGTEST")==0)
			obs= neg_test_obs;
		else if (strcmp(target,"TEST")==0)
			obs= test_obs;
		else
			CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");

		if (obs)
		{
			obs->set_dimension(dim);
		}
		else
			CIO::message("no observations were set for target %s\n",target);
	}
	else
	{
		CIO::message("target POSTRAIN|NEGTRAIN|POSTEST|NEGTEST|TEST missing\n");
		return false;
	}

	return true;
}

bool CGUIObservation::set_alphabet(char* param)
{
	char* input=CIO::skip_spaces(param);
	char target[1024];

	if ((sscanf(input, "%s", target))==1)
	{
		if (strcmp(target,"PROTEIN")==0)
			alphabet=PROTEIN;
		else if (strcmp(target,"ALPHANUM")==0)
			alphabet=ALPHANUM;
		else if (strcmp(target,"DNA")==0)
			alphabet=DNA;
		else if (strcmp(target,"CUBE")==0)
			alphabet=CUBE;
		else
		{
			CIO::message("unknown alphabet!\n");
			return false;
		}
		return true;
	}
	else
		CIO::message("see help for parameters\n");
	return false;
}

int CGUIObservation::get_alphabet_size()
{
	return CObservation::get_alphabet_size(alphabet);
}
