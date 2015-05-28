#ifndef VALUES_H_PWZ5324D
#define VALUES_H_PWZ5324D

#include <cstdint>
#include <thread>
#include <mutex>

template<typename T>
struct Triplet {
	T A;
	T B;
	T C;
};

typedef Triplet<double> DoubleTriplet;

struct PIDParam {
	double KP;
	double KI;
	double KD;
};

class Parameters {
public:

	static Parameters & getInstance(void) {
		static Parameters instance;
		return instance;
	}

	void SetParamsHaveChanged(bool val){
		mMutexParamsHaveChanged.lock();
		ParamsHaveChanged = val;
		mMutexParamsHaveChanged.unlock();
	}

	bool GetParamsHaveChanged(){
		bool val = false;
		mMutexParamsHaveChanged.lock();
		val = ParamsHaveChanged;
		mMutexParamsHaveChanged.unlock();
		return val;
	}

	// System Parameter
	int UMax;
	double DistancePropellor;
	double ThrustConst;
	double AirResistance;

	std::uint8_t MotorMin;
	std::uint8_t MotorMax;

	// Regel Parameter
	Triplet<PIDParam> PIDAngular;
	DoubleTriplet PIDAngularN;
	Triplet<PIDParam> PIDAngularRate;
	DoubleTriplet PIDAngularRateN;
	DoubleTriplet TauMax;

	// Steuerung
	DoubleTriplet SetPointAngular;
	double Thrust;
	double ThrustMin;
	double ThrustMax;
	double ThrustStep;

	// Sensoren
	DoubleTriplet GyroOffset;
	DoubleTriplet AccOffset;

private:
	Parameters() :
			ParamsHaveChanged(true), UMax(7000), DistancePropellor(0.22), ThrustConst(
					1.6155e-7), AirResistance(0.0000001), MotorMin(10), MotorMax(
					255), Thrust(1.0), // Starting Thrust, can be changed at runtime
			ThrustMin(0.1), ThrustMax(20), ThrustStep(0.02) {
		GyroOffset = {0.0, 0.0, 0.0};
		AccOffset = {0.0, 0.0, 0.0};
		TauMax = {20, 20, 5};
		SetPointAngular = {0.0, 0.0, 0.0};
		PIDAngular = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
		PIDAngularRate =
		{
			{
				100,
				0,
				20
			},
			{
				100,
				0,
				20
			},
			{
				0,
				0,
				0
			}
		};

		PIDAngularN = {1.0, 1.0, 1.0};
		PIDAngularRateN = {
			1,
			1,
			1
		};
	}
	Parameters(Parameters &);
	Parameters(Parameters const &);
	Parameters(Parameters &&);
	Parameters(Parameters const &&);
	Parameters & operator= (Parameters &);
	Parameters & operator= (Parameters const &);

	// variable that shows that Parameters where changed
	bool ParamsHaveChanged;
	std::mutex mMutexParamsHaveChanged;
};

#endif /* end of include guard: VALUES_H_PWZ5324D */

