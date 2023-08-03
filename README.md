# StCFMult

`version: 1.0`

`author: Yige Huang`

## Usage

1. Put your n sigma shift module into ShiftUtil.cxx.

2. Include `StCFMult.h`.

3. `StCFMult* multCounter = new StCFMult()`.

4. `if (multCounter->make(mPicoDst)) { refMult3 = multCounter->mRefMult3; }`

## Change Log

05.07.2023 by yghuang (v1.0):

> First version of StCFMult module.
