#pragma once

#include "./FeatVec_EXP.hpp"
using namespace Grusoft;

void FeatVec_EXP::EDA(const LiteBOM_Config&config, bool genHisto, int flag) {
	size_t i,nSamp_= nSamp();
	assert(hDistri == nullptr);
	hDistri = new Distribution();
	*hDistri = *hRight->hDistri;
	hDistri->nam = nam;

	HistoGRAM *histo_right = hRight->hDistri->histo;
	assert(histo_right!=nullptr);
	hDistri->histo = new HistoGRAM(this,nSamp_,0x0);
	hDistri->histo->CopyBins(*histo_right,true,0x0);
	//hDistri->STA_at(nSamp_, val, true, 0x0);
	if (ZERO_DEVIA(hDistri->vMin, hDistri->vMax))
		BIT_SET(this->type, Distribution::V_ZERO_DEVIA);	
	
}