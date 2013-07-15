#ifndef _MULTI_H_
#define _MULTI_H_

typedef struct IPlugMulti1_
{
	void PLUGDEF(Acquire)(void* lpSelf);
	void PLUGDEF(Release)(void* lpSelf);
}
IPlugMulti1;

#define IPlugMulti    IPlugMulti1

enum
{
	PATCH_KRO = 0,
	PATCH_RE,
	PATCH_CUSTOM,
};

#endif  /* _MULTI_H_ */
