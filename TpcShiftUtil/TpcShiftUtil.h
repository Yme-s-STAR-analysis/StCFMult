#ifndef __YIGE_TPCSHIFT__
#define __YIGE_TPCSHIFT__

class TH1F;
class TH2F;

class TpcShiftUtil {
    private:
        TH1F* shift1D;
        TH2F* shift2D;
    
    public:
        TpcShiftUtil(){}
        ~TpcShiftUtil(){}

        bool Init(const char* fname, const char* h1name, const char* h2name);
        double GetShift(double p);
        double GetShift(double pT, double y);
        void Close() {  shift1D->Close();   shift2D->Close(); }
};

#endif