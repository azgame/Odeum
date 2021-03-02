#ifndef SPACES_H
#define SPACES_H



#include <vector>
#include <list>


struct Spaces {

	void enter()
	{
		list<Trigger*>::iterator t = triggers.begin();

		while (t != triggers.end())
		{
			(*(*t))();
			t = triggers.erase(t);
		}

		void addTrigger(Trigger *t)
		{
			triggers.push_back(t);
		}







	};



	Trigger(const std::string& msg) : msg(msg)
	{ }

	void operator() ()
	{
		std::cout << msg;
	}

private:

	std::vector<Node*> nodes;
	list<Trigger*> triggers;

	



}
#endif
