#include "StCFMult.h"
#include "TMath.h"

StCFMult::StCFMult() {
    clean();
    shift = 0;
}

void StCFMult::clean() {
    mRefMult = 0;
    mRefMult3 = 0;
    mRefMult3X = 0;
    mNTofBeta = 0;
    mNTofMatch = 0;
    mNTofMatchZ = 0;
    mTofMult = 0;
}

void StCFMult::ImportShiftTool(TpcShiftTool* shift_ptr) {
    mtShift = shift_ptr;
}

void StCFMult::IgnoreShift() {
    shift = 0;
}

bool StCFMult::make(StPicoDst *picoDst) {
    clean();
    Int_t nTracks = picoDst->numberOfTracks();

    StPicoEvent* picoEvent = (StPicoEvent*)picoDst->event();
    if (!picoEvent) { return false; }

    Int_t runId = picoEvent->runId();
    TVector3 vertex = picoEvent->primaryVertex();
    Double_t vx = vertex.X();
    Double_t vy = vertex.Y();
    Double_t vz = vertex.Z();

    for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
        StPicoTrack* picoTrack = (StPicoTrack *)picoDst->track(iTrack);
        if (!picoTrack || !picoTrack->isPrimary()) { continue; }

        Int_t nHitsFit = picoTrack->nHitsFit();
        if (nHitsFit <= 10) { continue; }

        Int_t nHitsDedx = picoTrack->nHitsDedx();

        Double_t dca = fabs(picoTrack->gDCA(vx, vy, vz));
        if (dca > 3) { continue; }

        TVector3 pmomentum = picoTrack->pMom();
        Double_t pcm = pmomentum.Mag();
        Double_t pt = pmomentum.Perp();
        if (pcm < 1e-10) { continue; }
        Double_t eta = pmomentum.PseudoRapidity();
        Double_t nsig = picoTrack->nSigmaProton();
        if (shift != 0) { // apply n sigma shift
            nsig -= shift->GetShift(runId, pt, eta);
        }

        Int_t tofId = picoTrack->bTofPidTraitsIndex();
        Int_t btofMatchFlag = 0;
        Double_t beta = -1.0;
        if (tofId >= 0) {
            StPicoBTofPidTraits* tofPid = picoDst->btofPidTraits(tofId);
            btofMatchFlag = tofPid->btofMatchFlag();
            if (tofPid) {
                beta = tofPid->btofBeta();
            }
        }
        Double_t mass2 = -999;
        if (btofMatchFlag > 0 && beta > 1e-5) {
            mass2 = pcm * pcm * (pow(1.0 / beta, 2) - 1);
        }

        if (
            (nHitsDedx > 5 || mass2 > -990) &&
            nsig < -3 &&
            mass2 < 0.4 && 
            fabs(eta) < 1.0
        ) { 
            /*
                Important note for this cut: - From Fan Si
                    If nHitsDedx is less than 5, dE/dx quantity would fail to be calculated, 
                    and nSigmaProton will be negative infty and set to be -32768,
                    from getter of nSigmaProton, it will be -32.768 and naturally less than -3.
                    If one is not aware of this mechanism, he/she can not understand the logic here.
            */
            mRefMult3 += 1;
        }
        if (
            (nHitsDedx > 5 || mass2 > -990) &&
            nsig < -3 &&
            mass2 < 0.4 &&
            fabs(eta) < 1.6
        ) { 
            mRefMult3X += 1;
        }
        if (beta > 0.1 && beta < 1.1 && fabs(eta) < 1.0){
            mNTofBeta += 1;
        }
        if (fabs(eta) < 0.5 && btofMatchFlag > 0){
            mNTofMatchZ += 1;
        }
    }

    mRefMult = picoEvent->refMult();
    mNTofMatch = picoEvent->nBTOFMatch();
    mTofMult = picoEvent->btofTrayMultiplicity();
    return true;
}
