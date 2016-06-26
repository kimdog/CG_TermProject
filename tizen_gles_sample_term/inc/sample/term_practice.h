#ifndef TERM_PRACTICE_H_
#define TERM_PRACTICE_H_

#include "sample_view.h"

class TermPractice : public SampleView
{
public:
	TermPractice(void* data) : SampleView(data)
	{
	}

	void OnInit() override;

	void OnStep() override;
};




#endif /* TERM_PRACTICE_H_ */
