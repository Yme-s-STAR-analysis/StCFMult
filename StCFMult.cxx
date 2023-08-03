#include "StCFMult.h"

StCFMult::StCFMult(){
    mRefMult3 = 0;
    mBetaEta1 = 0;
    mNTofMatch = 0;
    mTofMult = 0;
    shift = new ShiftUtil();
}
~StCFMult::StCFMult(){}

void StCFMult::clean(){
    mRefMult3 = 0;
    mBetaEta1 = 0;
    mNTofMatch = 0;
    mTofMult = 0;
}

bool StCFMult::make(StPicoDst *picoDst){
    clean();
    Int_t nTracks = picoDst->numberOfTracks();

    StPicoEvent* picoEvent = (StPicoEvent*)picoDst->event();
    if (!picoEvent){
        return false;
    }

    Int_t runId = picoEvent->runId();
    TVector3 vertex = picoEvent->primaryVertex();
    Double_t vx = vertex.X();
    Double_t vy = vertex.Y();
    Double_t vz = vertex.Z();

    for (Int_t iTrack=0; iTrack<nTracks; iTrack++){
        StPicoTrack* picoTrack = (StPicoTrack *)picoDst->track(iTrack);
        if (!picoTrack || !picoTrack->isPrimary()){
            continue;
        }

        Int_t nHitsFit = picoTrack->nHitsFit();
        if (nHitsFit <= 10){
            continue;
        }

        Int_t nHitsDedx = picoTrack->nHitsDedx();

        Double_t dca = fabs(picoTrack->gDCA(vx, vy, vz));
        if (dca > 3){
            continue;
        }

        TVector3 pmomentum = picoTrack->pMom();
        Double_t pcm = pmomentum.Mag();
        if (pcm < 1e-10){
            continue;
        }
        Double_t eta = pmomentum.PseudoRapidity();
        if (fabs(eta) > 1.0){
            continue;
        }

        Int_t q = picoTrack->charge();
        Double_t nsig = picoTrack->nSigmaProton();
        // use selected n sigma shift member function
        nsig -= shift->GetShift(p);

        Int_t tofId = picoTrack->bTofPidTraitsIndex();
        Int_t btofMatchFlag = 0;
        Double_t beta = -1.0;
        if (tofId >= 0){
            StPicoBTofPidTraits* tofPid = picoDst->btofPidTraits(tofId);
            btofMatchFlag = tofPid->btofMatchFlag();
            if (tofPid){
                beta = tofPid->btofBeta();
            }
        }
        Double_t mass2 = -999;
        if (btofMatchFlag > 0 && beta > 1e-5){
            mass2 = pcm * pcm * (pow(1.0 / beta, 2) - 1);
        }

        if (
            (nHitsDedx > 5 || mass2 > -990) &&
            ((nHitsDedx > 5 && nsig < -3) || (mass2 > -990 && mass2 < 0.4))
        ) {
            mRefMult3 += 1;
        }
        if (beta > 0.1){
            mBetaEta1 += 1;
        }
        if (fabs(eta) < 0.5 && btofMatchFlag > 0){
            mNTofMatch += 1;
        }
    }
    mTofMult = picoEvent->btofTrayMultiplicity();
    return true;
}