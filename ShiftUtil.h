#ifndef __STSHIFT_HEADER__
#define __STSHIFT_HEADER__

class ShiftUtil {
    public:
        ShiftUtil(){}
        ~ShiftUtil(){}

        double GetShift(double p);
        double GetShift(double pT, double y);
        double GetShiftWithRun(int runId, double p);
        double GetShiftWithRun(int runId, double pT, double y);
};

#endif