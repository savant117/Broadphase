#include <iostream>
#include <iomanip>

#include "broadphase.h"
#include "nsq.h"
#include "util.h"

struct TestResults
{
	double init_time;
	double min_time;
	double max_time;
	double avg_time;
	size_t memory;
};

struct Test2D
{
	IBroadphase2D* bp;
	int iterations;

	TestResults run(IBroadphase2D* bp_)
	{
		TestResults r;
		r.min_time = DBL_MAX;
		r.max_time = -DBL_MAX;
		r.avg_time = 0;

		bp = bp_;
		bp->init();

		double t0 = timeMillis();
		init();
		r.init_time = timeMillis() - t0;

		for (int i = 0; i < iterations; i++)
		{
			t0 = timeMillis();
			step();
			double dt = timeMillis() - t0;

			r.min_time = min(r.min_time, dt);
			r.max_time = max(r.max_time, dt);
			r.avg_time += dt;

			postStep();
		}

		r.avg_time /= (double)iterations;
		r.memory = bp->measureMemory();

		return r;
	}

	virtual void init() = 0;
	virtual void step() = 0;
	virtual void postStep() { }
	virtual string toString() const = 0;
};

struct BasicTest : public Test2D
{
	int n;

	BasicTest(int n_)
		: n(n_)
	{ }

	void init()
	{
		iterations = 10;
		srand(17);
		for (int i = 0; i < n; i++)
			bp->add(new Object2D(aabb2(float2(randf(-1000, 1000), randf(-1000, 1000)), float2(randf(-1000, 1000), randf(-1000, 1000))), false));
	}

	void step()
	{
		bp->findPairs();
	}

	string toString() const
	{
		return "BasicTest (n = " + to_string(n) + ")";
	}
};

void main()
{
	vector<Test2D*> tests;
	vector<IBroadphase2D*> broadphases;

	tests.push_back(new BasicTest(10));
	tests.push_back(new BasicTest(100));
	tests.push_back(new BasicTest(1000));

	broadphases.push_back(new NSq2D());

	for (auto t : tests)
	{
		cout << t->toString() << "\n------------------------------\n";
		cout << "structure\tinit\tavg\tmin\tmax\tmemory\n";

		for (auto b : broadphases)
		{
			auto r = t->run(b);
			cout << b->toString() << setprecision(2) << "\t\t" << r.init_time << "\t" << r.avg_time << "\t" << r.min_time << "\t" << r.max_time << "\t" << r.memory << "\n";
		}

		cout << "\n\n";
	}

	cin.get();
}