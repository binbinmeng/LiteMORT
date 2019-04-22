// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PYMORT_DLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PYMORT_DLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PYMORT_DLL_EXPORTS
#define PYMORT_DLL_API __declspec(dllexport)
#else
#define PYMORT_DLL_API __declspec(dllimport)
#endif
#include "../util/GST_def.h"

#define __API_BEGIN__() try {

#define __API_END__() } \
catch(std::exception& ex) { return (ex); } \
catch(std::string& ex) { return (ex); } \
catch(...) { return ("unknown exception"); } \
return 0;


struct PY_ITEM {
	char *Keys;
	float Values;
	char *text;
	//int Index;
};


#ifdef __cplusplus
extern "C" {
#endif
	PYMORT_DLL_API void* LiteMORT_init(PY_ITEM* params, int nParam, int flag);
	PYMORT_DLL_API void LiteMORT_clear(void*);
	//PYMORT_DLL_API void LiteMORT_set_feat(PY_ITEM* params, int nParam, int flag);
	PYMORT_DLL_API void LiteMORT_fit(void *,float *h_data, tpY *h_target, size_t nSamp, size_t ldS, float *eval_data, tpY *eval_target, size_t nEval, size_t flag);
	PYMORT_DLL_API void LiteMORT_predict(void *,float *X, tpY *y, size_t nFeat_0, size_t nSamp, size_t flag);
	PYMORT_DLL_API void LiteMORT_Imputer_f(float *X, tpY *y, size_t nFeat_0, size_t nSamp, size_t flag);
	PYMORT_DLL_API void LiteMORT_Imputer_d(double *X, tpY *y, size_t nFeat_0, size_t nSamp, size_t flag);
	PYMORT_DLL_API void LiteMORT_EDA(void *, const float *X, const tpY *y, const size_t nFeat_0, const size_t nn, const size_t nValid,
		PY_ITEM* params, int nParam, const size_t flag);
#ifdef __cplusplus
}
#endif
