#pragma once
#include <vector>
#include <map>  
#include <algorithm>
#include <cmath>
#include "../util/samp_set.hpp"

using namespace std;
typedef map<int, int> MAP_CATEGORY;


/*
	https://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
	�Ƚϸ��ӣ���ʱ����
*/
template <typename T>
void sort_indexes_NA(int nSamp, const T*v, vector<size_t>& idx, int flag = 0x0) {
	size_t i, nz0=0, nGood=0,*map=new size_t[nSamp];
	T *vGood =new T[nSamp];
	for (i = 0; i < nSamp; i++) {
		if (IS_NAN_INF(v[i])) {
			continue;
		}	else {
			vGood[nGood] = v[i];		map[nGood]= i;		nGood++;
		}
	}
	vector<size_t> idx_1;
	idx_1.resize(nGood);// initialize original index locations
	iota(idx_1.begin(), idx_1.end(), 0);
	// sort indexes based on comparing values in v
	std::sort(idx_1.begin(), idx_1.end(), [&vGood](size_t i1, size_t i2) {return vGood[i1] < vGood[i2]; });
	for (i = 0; i < nGood-1; i++) {
		assert(vGood[idx_1[i]] <= vGood[idx_1[i + 1]]);
	}
	idx.resize(nSamp);
	for (i = 0; i < nGood; i++) {
		idx[i] = map[idx_1[i]];
	}
	for (i = 0; i < nSamp; i++) {
		if (IS_NAN_INF(v[i])) {
			idx[nGood++]=i;
		}		
	}
	assert(nGood==nSamp);
	delete[] vGood;			delete[] map;
	return;
}

template <typename T>
void sort_indexes(int nSamp,const T*v, vector<tpSAMP_ID>& idx, int flag = 0x0) {
	if (false) {
		//sort_indexes_NA(nSamp,v,idx,flag);
		return;
	}
		
	size_t i;
	idx.resize(nSamp);// initialize original index locations
	iota(idx.begin(), idx.end(), 0);
	// sort indexes based on comparing values in v
	std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
	for (i = 0; i < nSamp - 1; i++) {
		assert(v[idx[i]] <= v[idx[i + 1]]);
	}
	return;
}

template <typename T>
void sort_indexes(const vector<T> &v, vector<tpSAMP_ID>& idx, int flag = 0x0) {
	T *arr = (T*)(v.data());
	sort_indexes(v.size(),arr, idx,  flag);	
	return;
}



namespace Grusoft {
	class FeatsOnFold;
	class FeatVector;
	typedef short tpQUANTI;
	//���ڿ���split,histogram��ʵ����򵥵�GreedyCluster
	class GreedyCluster {

	};

	class HistoGRAM;

	//feature vector���������Ͷ�䣬����ӳ��ܸ���
	struct HISTO_BIN {
		size_t nz = 0;
		//if (val[pos] < bins[noBin+1].split_F) {		quanti[pos] = noBin;		}
		double split_F = 0;		
		char fold = -1;			//ÿ��BIN����һ��FOLD
		int tic = 0;			//X		��ɾ��
		//tpQUANTI split_Q = -1;	//����֮��
		double G_sum = 0, H_sum = 0;		//Second-order approximation from Tianqi Chen's formula

		static bool isSplitSmall(const HISTO_BIN &l, const HISTO_BIN &r)
		{
			return l.split_F<r.split_F;
		}

		
	};

	//HISTO_BIN����¼������Ϣ feat analysis is here...
	struct BIN_FEATA {
		double split_F = 0;
		char fold = -1;			//ÿ��BIN����һ��FOLD
		int tic = 0;			//X		��ɾ��
		double density, bandwidth;		//The bandwidth of the kernel is a free parameter which exhibits a strong influence on the resulting estimate
	};

	class FRUIT{
	public:
		FRUIT(HistoGRAM *his_ = nullptr,int flag=0x0) ;
		HistoGRAM *histo = nullptr;		//��ָ�򣬲���ɾ��
		HISTO_BIN bin_S0, bin_S1;						//�б仯���Ƚ�Σ����
		double adaptive_thrsh;					//��ָ��binSplit�м䣬���Ա���		splitonYҲ��Ҫ���ֵ
		double Thrshold(bool isQuanti) {
			//assert(binSplit.split_Q >= 0);
			if (isQuanti) {
				return bin_S1.tic;
			}	else {
				if(true)
					return adaptive_thrsh;
				else 	
					return bin_S1.split_F;
			}
		}
		//tpQUANTI T_quanti = 0;
		MAP_CATEGORY mapCategory;
		//��Ҫ������ƣ��μ�SplitOn�Լ�AtLeaf����
		bool isY = false;
		bool isNanaLeft = false;
		//size_t pos_0 = 0, pos_1 = 0;
		size_t nLeft = 0, nRight = 0;
		double tic_left = 0;
		double mxmxN = -1;		//mean*mean*N
		std::string sX;		//���ڵ�����Ϣ

		//char buf[256] = { 0 };		//https://stackoverflow.com/questions/4523497/typedef-fixed-length-array	Arrays can't be passed as function parameters by value in C.
		//double lft_impuri,rgt_impuri;		//Ϊ�˵���
		virtual ~FRUIT();
		/*
		�μ�FeatVec_T::SplitOn
		
		template<typename Tx>
		Tx Get_() {
			assert(histo == nullptr);
			Tx a = thrshold;
			//a = *reinterpret_cast<double*>(buf);		//https://stackoverflow.com/questions/15464427/cast-char-to-double-as-bytes
			//memcpy(&a, buf, sizeof(Tx));
			return a;
		}*/
		
	};

	
	/*
		���ض�FeatVector��Ӧ��HistoGRAM
	*/
	class HistoGRAM {
	protected:
		//FeatVector*	hFeat = nullptr;
	public:
		typedef enum {
			SPLIT_BY_VALUE=0x0,SPLIT_BY_DENSITY
		}SPLIT_FEAT;
		SPLIT_FEAT split= SPLIT_BY_VALUE;

		size_t nSamp, nLeft = 0, nRight=0;
		FRUIT *fruit=nullptr;			//����ָ��
		FeatVector *hFeat = nullptr;	//����ָ��
		//tpQUANTI *quanti=nullptr;	//ָ��qHisto->quanti(�μ�FeatVec_Q::UpdateHisto)������ɾ��
		//double a0 = 0, a1 = 0,split;
		vector<HISTO_BIN> bins;
		//NA value---������ĳfeatrueȷʵmissing value ,�������ϻ�����down direction
		HISTO_BIN* hBinNA( )		
		{	return &(bins[bins.size()-1]);	 }		//���Ƿ������

		HistoGRAM(FeatVector*hFeat_,size_t nMost, int flag = 0x0) : hFeat(hFeat_){
			nSamp = nMost;
			//a1 = -DBL_MAX, a0 = DBL_MAX;	
		}
		virtual ~HistoGRAM();		

		virtual void CompressBins(int flag=0x0);

		HistoGRAM* FromDiff(const HistoGRAM*hP, const HistoGRAM*hB, int flag = 0x0);

		template<typename Tx,typename Tf>
		bool At(Tx x, Tf&f,int flag = 0x0) {
			double rigt=DBL_MAX;
			bool isFind=false;
			size_t nBin = bins.size(),i;
			for (i=0;i<nBin;i++) {
				rigt = i<nBin-1 ? bins[i+1].tic : DBL_MAX;
				const HISTO_BIN& bin= bins[i];
				if(bin.nz>0 && bin.tic<=x && x<rigt)				{	
					f = bin.G_sum/ bin.nz;
					isFind = true;		break;
				}
				
			}
			return isFind;
		}

		template<typename Tx>
		int AtBin_(Tx x, int flag = 0x0) {
			double rigt = DBL_MAX;
			//bool isFind = false;
			int pos=-1;
			size_t nBin = bins.size(), i;
			for (i = 0; i<nBin; i++) {
				rigt = i<nBin - 1 ? bins[i + 1].tic : DBL_MAX;
				const HISTO_BIN& bin = bins[i];
				if (bin.nz>0 && bin.tic <= x && x<rigt) {
				//if (bin.tic <= x && x<rigt) {
					return i;
				}

			}
			return pos;
		}

	//Sturge��s Rule	http://www.statisticshowto.com/choose-bin-sizes-statistics/
	//Histogram Binwidth Optimization Method	http://176.32.89.45/~hideaki/res/histogram.html
		//Optimal Data-Based Binning for Histograms
		virtual void OptimalBins(size_t nMost, size_t nSamp, double a0_,double a1_,int flag=0x0){
			bins.clear();
			if(a0_==a1_)
				return;
			if (nMost == 0) {
				//nMost = 3.49*sigma/pow(nSamp, 0.333);	//Rice��s Rule
				nMost = 1 + 2*pow(nSamp,0.333);	//Rice��s Rule
				nMost = 1 + 3.322*log(nSamp);	//Sturge��s Rule
			}
			assert(nMost >=2 );
			bins.resize(nMost);
			//a1 = a1_,		a0 = a0_;
		}

		/*����Samp2Histo_qsort�е��ã�Ӧɾ��*/
		virtual void ReSet(size_t nMost,int flag=0x0){
			nSamp = nMost;
			bins.clear();
			bins.resize(nMost);
			//a1 = -DBL_MAX, a0 = DBL_MAX;
		}

		virtual void MoreHisto(const FeatsOnFold *hData_, vector<HistoGRAM*>&more, int flag = 0x0);
		virtual void GreedySplit_X(const FeatsOnFold *hData_, const SAMP_SET& samp_set, int flag = 0x0);
		//virtual void GreedySplit_X(const FeatsOnFold *hData_, const SAMP_SET& samp_set, int flag = 0x0);
		virtual void GreedySplit_Y(const FeatsOnFold *hData_, const SAMP_SET& samp_set,bool tryX, int flag = 0x0);
		virtual void Regress(const FeatsOnFold *hData_, const SAMP_SET& samp_set,  int flag = 0x0);

		virtual void CopyBins(const HistoGRAM &src,bool isReset,int flag) {
			//nSamp = src.nSamp;
			bins = src.bins;
			if (isReset) {
				for (int i=0;i<bins.size();i++){
					HISTO_BIN &item = bins[i];
					item.nz=0;
					//item.Y_sum = 0;		
					item.H_sum = 0;		item.G_sum = 0;
				}
			}
			//a1 = -DBL_MAX, a0 = DBL_MAX;
		}
	};
	//typedef map<int, HistoGRAM*> MAP_HistoGRAM;
	typedef vector<HistoGRAM*> H_HistoGRAM;

	/*
		from Tianqi Chen's formula
	
	class Histo_CTQ : public HistoGRAM {
	protected:
		//double gSum=0,hSum=0;
	public:
		Histo_CTQ(size_t nMost, int flag = 0x0) : HistoGRAM(nMost,flag){
		}

		virtual void GreedySplit_X(const FeatsOnFold *hData_, const SAMP_SET& samp_set, int flag = 0x0);
		//virtual void GreedySplit_Y(const FeatsOnFold *hData_, const SAMP_SET& samp_set, bool tryX = true, int flag = 0x0) { ; }
		virtual void Regress(const FeatsOnFold *hData_, const SAMP_SET& samp_set, int flag = 0x0) { ; }
	};*/
}